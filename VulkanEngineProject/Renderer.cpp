#include <Zodiacpch.h>
#include "Renderer.h"
#include "Initializers.h"
#include "Validation.h"
#include "VulkanShaderModule.h"
#include "Vertex.h"

Zodiac::Renderer* Zodiac::Renderer::s_instance = nullptr;
Zodiac::VulkanDevice* Zodiac::Renderer::s_device;
Zodiac::VulkanSurface* Zodiac::Renderer::s_surface;
Zodiac::Settings Zodiac::Renderer::s_settings;
Zodiac::VulkanSwapchain* Zodiac::Renderer::s_swapchain;
VkRenderPass Zodiac::Renderer::s_renderPass;
VkPipeline Zodiac::Renderer::s_pipeline;
VkPipelineCache Zodiac::Renderer::s_pipelineCache;
VkPipelineLayout Zodiac::Renderer::s_pipelineLayout;
std::vector<VkFramebuffer> Zodiac::Renderer::s_framebuffers;
std::vector<VkCommandBuffer> Zodiac::Renderer::s_drawCmdBuffers;
Zodiac::VulkanSemaphore* Zodiac::Renderer::s_presentSemaphore;
Zodiac::VulkanSemaphore* Zodiac::Renderer::s_renderCompleteSemaphore;
std::vector<Zodiac::VulkanFence*> Zodiac::Renderer::s_waitFences;

void Zodiac::Renderer::DrawIndexed() {

}

Zodiac::Renderer::Renderer() {
	s_instance = this;
}

Zodiac::Renderer::~Renderer() {

}

void Zodiac::Renderer::Init(VulkanDevice* device, Settings settings, VulkanSurface* surface) {
	s_device = device;
	s_settings = settings;
	s_surface = surface;

	InitInternal();
}

Zodiac::Renderer& Zodiac::Renderer::Get() {
	return *s_instance;
}

void Zodiac::Renderer::InitInternal() {
	s_swapchain = new Zodiac::VulkanSwapchain(s_device, s_surface->GetSurfaceDetails(), s_surface->GetSurface(), s_settings);
	SetupRenderPass();
	SetupPipelineCache();
	SetupFramebuffers();

	s_presentSemaphore = new Zodiac::VulkanSemaphore(s_device);
	s_renderCompleteSemaphore = new Zodiac::VulkanSemaphore(s_device);
	s_drawCmdBuffers.resize(s_swapchain->GetImageCount());
	s_waitFences.reserve(s_drawCmdBuffers.size());
	for (size_t i = 0; i < s_drawCmdBuffers.size(); i++) {
		s_waitFences.emplace_back(new VulkanFence(s_device));
	}

	SetupPipeline();
	PrepareGeometry();
}

void Zodiac::Renderer::SetupRenderPass() {
	// Descriptors for the attachments used by this renderpass
	std::vector<VkAttachmentDescription> attachments(2);

	// Color attachment
	attachments[0].format = s_swapchain->GetSurfaceFormat().format;					// Use the color format selected by the swapchain
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear this attachment at the start of the render pass
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;							// Keep it's contents after the render pass is finished (for displaying it)
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// We don't use stencil, so don't care for load
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;				// Same for store
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;					// Layout to which the attachment is transitioned when the render pass is finished
																					// As we want to present the color buffer to the swapchain, we transition to PRESENT_KHR
	// Depth attachment
	attachments[1].format = s_swapchain->GetDepthFormat();
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear depth at start of first subpass
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;						// We don't need depth after render pass has finished (DONT_CARE may result in better performance)
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// No stencil
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;				// No Stencil
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;	// Transition to depth/stencil attachment

	// Setup attachment references
	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;													// Attachment 0 is color
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;				// Attachment layout used as color during the subpass

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;													// Attachment 1 is depth
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;		// Attachment used as depth/stemcil used during the subpass

	// Setup a single subpass reference
	std::vector<VkSubpassDescription> subpassDescriptions(1);
	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].colorAttachmentCount = 1;									// Subpass uses one color attachment
	subpassDescriptions[0].pColorAttachments = &colorReference;							// Reference to the color attachment in slot 0
	subpassDescriptions[0].pDepthStencilAttachment = &depthReference;					// Reference to the depth attachment in slot 1
	subpassDescriptions[0].inputAttachmentCount = 0;									// Input attachments can be used to sample from contents of a previous subpass
	subpassDescriptions[0].pInputAttachments = nullptr;
	subpassDescriptions[0].preserveAttachmentCount = 0;									// Preserved attachments can be used to loop (and preserve) attachments through subpasses
	subpassDescriptions[0].pPreserveAttachments = nullptr;
	subpassDescriptions[0].pResolveAttachments = nullptr;								// Resolve attachments are resolved at the end of a sub pass and can be used for e.g. multi sampling

	// Setup subpass dependencies
	// These will add the implicit attachment layout transitionss specified by the attachment descriptions
	// The actual usage layout is preserved through the layout specified in the attachment reference		
	// Each subpass dependency will introduce a memory and execution dependency between the source and destination subpass described by
	// srcStageMask, dstStageMask, srcAccessMask, dstAccessMask (and dependencyFlags is set)
	// Note: VK_SUBPASS_EXTERNAL is a special constant that refers to all commands executed outside of the actual renderpass)
	std::vector<VkSubpassDependency> dependencies(2);

	// First dependency at the start of the renderpass
	// Does the transition from final to initial layout 
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;								// Producer of the dependency 
	dependencies[0].dstSubpass = 0;													// Consumer is our single subpass that will wait for the execution depdendency
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// Second dependency at the end the renderpass
	// Does the transition from the initial to the final layout
	dependencies[1].srcSubpass = 0;													// Producer of the dependency is our single subpass
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;								// Consumer are all commands outside of the renderpass
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	// Create the actual renderpass
	VkRenderPassCreateInfo renderPassInfo = Initializers::RenderPassCreateInfo(attachments, subpassDescriptions, dependencies);
	ErrorCheck(vkCreateRenderPass(*s_device->GetDevice(), &renderPassInfo, nullptr, &s_renderPass));
}

void Zodiac::Renderer::SetupPipelineCache() {
	VkPipelineCacheCreateInfo pipelineCacheInfo = Initializers::PipelineCacheCreateInfo();
	ErrorCheck(vkCreatePipelineCache(*s_device->GetDevice(), &pipelineCacheInfo, nullptr, &s_pipelineCache));
}

void Zodiac::Renderer::SetupFramebuffers() {
	s_framebuffers.resize(s_swapchain->GetImageCount());
	for (size_t i = 0; i < s_framebuffers.size(); i++) {
		std::vector<VkImageView> attachments(2);
		attachments[0] = s_swapchain->GetBuffers()[i].view;							// Color attachment is the view of the swapchain image			
		attachments[1] = s_swapchain->GetDepthStencil().view;						// Depth/Stencil attachment is the same for all frame buffers

		VkFramebufferCreateInfo framebufferInfo = Initializers::FramebufferCreateInfo(s_renderPass, attachments, s_swapchain->GetExtent2D().width, s_swapchain->GetExtent2D().height);
		ErrorCheck(vkCreateFramebuffer(*s_device->GetDevice(), &framebufferInfo, nullptr, &s_framebuffers[i]));
	}
}

bool Zodiac::Renderer::SetupPipeline() {
	VulkanShaderModule vertexShader(s_device, "Shaders/vert.spv");
	VulkanShaderModule fragmentShader(s_device, "Shaders/frag.spv");

	if ((vertexShader.GetShaderModule() == nullptr) || (fragmentShader.GetShaderModule() == nullptr)) {
		return false;
	}

	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos = {
		{ Initializers::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, *vertexShader.GetShaderModule()) },
		{ Initializers::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, *fragmentShader.GetShaderModule()) }
	};

	std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = {
		{ Initializers::VertexInputBindingDescription(0, sizeof(SimpleVertex)) }
	};

	std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions = {
		{ Initializers::VertexInputAttributeDescription(0, vertexBindingDescriptions[0].binding, 0) },
		{ Initializers::VertexInputAttributeDescription(1, vertexBindingDescriptions[0].binding, offsetof(struct SimpleVertex, color)) }
	};

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = Initializers::PipelineVertexInputStateCreateInfo(vertexBindingDescriptions, vertexInputAttributeDescriptions);
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = Initializers::PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = Initializers::PipelineViewportStateCreateInfo();
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = Initializers::PipelineRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = Initializers::PipelineMultisampleStateCreateInfo();

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		VK_FALSE,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = Initializers::PipelineColorBlendStateCreateInfo(colorBlendAttachmentState, 1);
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = Initializers::PipelineDepthStencilStateCreateinfo();

	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = Initializers::PipelineDynamicStateCreateInfo(dynamicStates);

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = Initializers::PipelineLayoutCreateInfo();
	ErrorCheck(vkCreatePipelineLayout(*s_device->GetDevice(), &pipelineLayoutCreateInfo, nullptr, &s_pipelineLayout));
	if (!s_pipelineLayout) {
		return false;
	}

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = Initializers::GraphicsPipelineCreateInfo(shaderStageCreateInfos, vertexInputStateCreateInfo, inputAssemblyStateCreateInfo, viewportStateCreateInfo, rasterizationStateCreateInfo, multisampleStateCreateInfo, colorBlendStateCreateInfo, dynamicStateCreateInfo, s_pipelineLayout, s_renderPass, depthStencilStateCreateInfo);
	ErrorCheck(vkCreateGraphicsPipelines(*s_device->GetDevice(), s_pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &s_pipeline));

	return true;
}

void Zodiac::Renderer::PrepareGeometry() {
	SimpleVertex* vertArr = new SimpleVertex[3];
	vertArr[0] = { glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
	vertArr[1] = { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
	vertArr[2] = { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f) };

	std::vector<uint32_t> indexBuffer = { 0, 1, 2 };

	struct StagingBuffer {
		VkDeviceMemory memory;
		VkBuffer buffer;
	};

	struct {
		StagingBuffer vertices;
		StagingBuffer indices;
	} stagingBuffers;

	Zodiac::VulkanBuffer* stagingBuffer = new Zodiac::VulkanBuffer(s_device, vertArr, sizeof(SimpleVertex), 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	stagingBuffer->SetData();

	delete[] vertArr;
	delete stagingBuffer;

	//Zodiac::VulkanBuffer* vertexBuffer = new Zodiac::VulkanBuffer(s_device, vertArr, sizeof(TestVertex), 3, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

	//delete vertexBuffer;

	//Tests
	s_device->GetGraphicsCommand(s_drawCmdBuffers.data(), s_swapchain->GetImageCount());

	s_device->FreeGraphicsCommand(s_drawCmdBuffers.data(), s_swapchain->GetImageCount());
	///////
}

void Zodiac::Renderer::Shutdown() {
	for (size_t i = 0; i < s_waitFences.size(); i++) {
		delete s_waitFences[i];
	}
	delete s_presentSemaphore;
	delete s_renderCompleteSemaphore;

	for (size_t i = 0; i < s_framebuffers.size(); i++) {
		vkDestroyFramebuffer(*s_device->GetDevice(), s_framebuffers[i], nullptr);
	}
	vkDestroyPipelineCache(*s_device->GetDevice(), s_pipelineCache, nullptr);
	vkDestroyPipelineLayout(*s_device->GetDevice(), s_pipelineLayout, nullptr);
	vkDestroyRenderPass(*s_device->GetDevice(), s_renderPass, nullptr);
	vkDestroyPipeline(*s_device->GetDevice(), s_pipeline, nullptr);
	delete s_swapchain;
}
