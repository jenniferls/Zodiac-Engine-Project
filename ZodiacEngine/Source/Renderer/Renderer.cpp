#include <Zodiacpch.h>
#include "Renderer.h"
#include "Defines.h"
#include "Initializers.h"
#include "Validation.h"
#include "VulkanShaderModule.h"
#include "Vertex.h"

const int Zodiac::Renderer::MAX_FRAMES_IN_FLIGHT;

void Zodiac::Renderer::DrawIndexed() {

}

Zodiac::Renderer::~Renderer() {

}

void Zodiac::Renderer::Init(VulkanDevice* device, Settings settings, VulkanSurface* surface, VulkanInstance* instance, Window* window) {
	m_device = device;
	m_settings = settings;
	m_surface = surface;

	m_window = window;

	m_clearValues[0].color = { 0.2f, 0.0f, 0.2f, 1.0f };
	m_clearValues[1].depthStencil = { 1.0f, 0 };
	
	m_currentFrame = 0;

	m_prepared = false;

	InitInternal();

	if (m_showGui) {
		m_imgui = std::unique_ptr<ImGuiLayer>(ImGuiLayer::Create());
		m_imgui->Init((GLFWwindow*)window->GetNativeWindow(), device, instance);
	}
}

Zodiac::Renderer& Zodiac::Renderer::Get() {
	static Renderer instance;
	return instance;
}

void Zodiac::Renderer::Draw(float dt, Camera* mainCamera) {
	vkWaitForFences(*m_device->GetDevice(), 1, &m_waitFences[m_currentFrame]->p_fence, VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult res = vkAcquireNextImageKHR(*m_device->GetDevice(), *m_swapchain->GetSwapchain(), UINT64_MAX, m_presentSemaphores[m_currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &imageIndex);

	UpdateUniformBuffers(imageIndex, dt, mainCamera);

	if (res == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		return;
	}
	else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	//Waiting here helps with syncing command buffer access TODO: Create multiple command buffers and record them during runtime
	if (m_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		ErrorCheck(vkWaitForFences(*m_device->GetDevice(), 1, &m_imagesInFlight[imageIndex]->p_fence, VK_TRUE, UINT64_MAX));
	}
	m_imagesInFlight[imageIndex] = m_waitFences[m_currentFrame];

	//Syncronization
	ErrorCheck(vkResetFences(*m_device->GetDevice(), 1, &m_waitFences[m_currentFrame]->p_fence));

	//This part is messy right now. Should try async instead and also make helper functions
	if (m_showGui) {
		RecordCommandBuffer(imageIndex);
		m_imgui->UpdateGUI(mainCamera);
		VkCommandBuffer imguiCommandBuffer = m_imgui->PrepareCommandBuffer(imageIndex);
		VkCommandBuffer commandBuffers[] = { m_drawCmdBuffers[imageIndex], imguiCommandBuffer };

		// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkSubmitInfo submitInfo = Initializers::SubmitInfo(m_presentSemaphores[m_currentFrame]->GetSemaphore(), m_renderCompleteSemaphores[imageIndex]->GetSemaphore(), &commandBuffers[0], 2, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		ErrorCheck(vkQueueSubmit(*m_device->GetGraphicsQueue(), 1, &submitInfo, m_waitFences[m_currentFrame]->p_fence));
	}
	else {
		RecordCommandBuffer(imageIndex, true);
		//Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkSubmitInfo submitInfo = Initializers::SubmitInfo(m_presentSemaphores[m_currentFrame]->GetSemaphore(), m_renderCompleteSemaphores[imageIndex]->GetSemaphore(), &m_drawCmdBuffers[imageIndex], 1, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		ErrorCheck(vkQueueSubmit(*m_device->GetGraphicsQueue(), 1, &submitInfo, m_waitFences[m_currentFrame]->p_fence));
	}

	VkPresentInfoKHR presentInfo = Initializers::PresentInfo(*m_swapchain->GetSwapchain(), imageIndex, m_renderCompleteSemaphores[imageIndex]->GetSemaphore());
	res = vkQueuePresentKHR(*m_device->GetGraphicsQueue(), &presentInfo);
	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || m_swapchainDirty) {
		m_swapchainDirty = false;
		RecreateSwapChain();
	}

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

//TODO:Create wrappers for Info structures
void Zodiac::Renderer::BeginDynamicRendering(VkCommandBuffer commandBuffer, int imageIndex, VkClearValue* pClearColor, VkClearValue* pDepthValue) {
	VkRenderingAttachmentInfoKHR ColorAttachment = {
	.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
	.pNext = NULL,
	.imageView = m_swapchain->GetBuffers()[imageIndex].view,
	.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	.resolveMode = VK_RESOLVE_MODE_NONE,
	.resolveImageView = VK_NULL_HANDLE,
	.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	.loadOp = pClearColor ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
	.storeOp = VK_ATTACHMENT_STORE_OP_STORE
	};

	if (pClearColor) {
		ColorAttachment.clearValue = *pClearColor;
	}

	VkRenderingAttachmentInfo DepthAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.imageView = m_swapchain->GetDepthStencil().view, //Depth stencil is the same for all framebuffers according to earlier comment
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.resolveImageView = VK_NULL_HANDLE,
		.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.loadOp = pDepthValue ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	};

	if (pDepthValue) {
		DepthAttachment.clearValue = *pDepthValue;
	}

	VkRenderingInfoKHR RenderingInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
		.renderArea = { {0, 0}, {(uint32_t)m_swapchain->GetExtent2D().width, (uint32_t)m_swapchain->GetExtent2D().height} },
		.layerCount = 1,
		.viewMask = 0,
		.colorAttachmentCount = 1,
		.pColorAttachments = &ColorAttachment,
		.pDepthAttachment = &DepthAttachment
	};

	vkCmdBeginRendering(commandBuffer, &RenderingInfo);
}

void Zodiac::Renderer::InitInternal() {
	m_swapchain = new Zodiac::VulkanSwapchain(m_device, m_surface->GetSurfaceDetails(), m_surface->GetSurface(), m_settings);
	SetupRenderPass();
	SetupPipelineCache();
	SetupFramebuffers();
	CreateSyncObjects();

	PrepareGeometry();
	PrepareUniformBuffers();
	SetupDescriptorSets();
	SetupPipeline();
	SetupDescriptorPool();
	PrepareDescriptorSet();
	//BuildCommandBuffers();
	AllocateCommandBuffers();
	m_prepared = true;
}

void Zodiac::Renderer::SetupRenderPass() {
	// Descriptors for the attachments used by this renderpass
	std::vector<VkAttachmentDescription> attachments(2);

	// Color attachment
	attachments[0].format = m_swapchain->GetSurfaceFormat().format;					// Use the color format selected by the swapchain
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear this attachment at the start of the render pass
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;							// Keep it's contents after the render pass is finished (for displaying it)
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// We don't use stencil, so don't care for load
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;				// Same for store
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;			// Layout to which the attachment is transitioned when the render pass is finished
																					// As we want to present the color buffer to the swapchain, we transition to PRESENT_KHR
	// Depth attachment
	attachments[1].format = m_swapchain->GetDepthFormat();
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
	ErrorCheck(vkCreateRenderPass(*m_device->GetDevice(), &renderPassInfo, nullptr, &m_renderPass));
}

void Zodiac::Renderer::SetupPipelineCache() {
	VkPipelineCacheCreateInfo pipelineCacheInfo = Initializers::PipelineCacheCreateInfo();
	ErrorCheck(vkCreatePipelineCache(*m_device->GetDevice(), &pipelineCacheInfo, nullptr, &m_pipelineCache));
}

void Zodiac::Renderer::SetupFramebuffers() {
	m_framebuffers.resize(m_swapchain->GetImageCount());
	for (size_t i = 0; i < m_framebuffers.size(); i++) {
		std::vector<VkImageView> attachments(2);
		attachments[0] = m_swapchain->GetBuffers()[i].view;							// Color attachment is the view of the swapchain image			
		attachments[1] = m_swapchain->GetDepthStencil().view;						// Depth/Stencil attachment is the same for all frame buffers

		VkFramebufferCreateInfo framebufferInfo = Initializers::FramebufferCreateInfo(m_renderPass, attachments, m_swapchain->GetExtent2D().width, m_swapchain->GetExtent2D().height);
		ErrorCheck(vkCreateFramebuffer(*m_device->GetDevice(), &framebufferInfo, nullptr, &m_framebuffers[i]));
	}
}

bool Zodiac::Renderer::SetupPipeline() {
	VulkanShaderModule vertexShader(m_device, (std::string(SHADERS_DIR) + "/triangle.vert.spv").c_str());
	VulkanShaderModule fragmentShader(m_device, (std::string(SHADERS_DIR) + "/triangle.frag.spv").c_str());

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
		{ Initializers::VertexInputAttributeDescription(1, vertexBindingDescriptions[0].binding, offsetof(SimpleVertex, color)) }
	};

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = Initializers::PipelineVertexInputStateCreateInfo(vertexBindingDescriptions, vertexInputAttributeDescriptions);
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = Initializers::PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = Initializers::PipelineViewportStateCreateInfo();
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = Initializers::PipelineRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = Initializers::PipelineMultisampleStateCreateInfo();

	//VkPipelineColorBlendAttachmentState colorBlendAttachmentState = { //No blend
	//	VK_FALSE,
	//	VK_BLEND_FACTOR_ONE,
	//	VK_BLEND_FACTOR_ZERO,
	//	VK_BLEND_OP_ADD,
	//	VK_BLEND_FACTOR_ONE,
	//	VK_BLEND_FACTOR_ZERO,
	//	VK_BLEND_OP_ADD,
	//	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
	//	VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	//};
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {}; //Alpha blend
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = VK_TRUE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = Initializers::PipelineColorBlendStateCreateInfo(colorBlendAttachmentState, 1);
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = Initializers::PipelineDepthStencilStateCreateinfo();

	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = Initializers::PipelineDynamicStateCreateInfo(dynamicStates);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = Initializers::GraphicsPipelineCreateInfo(shaderStageCreateInfos, vertexInputStateCreateInfo, inputAssemblyStateCreateInfo, viewportStateCreateInfo, rasterizationStateCreateInfo, multisampleStateCreateInfo, colorBlendStateCreateInfo, dynamicStateCreateInfo, m_pipelineLayout, m_renderPass, depthStencilStateCreateInfo);
	ErrorCheck(vkCreateGraphicsPipelines(*m_device->GetDevice(), m_pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline));

	return true;
}

void Zodiac::Renderer::PrepareGeometry() {
	SimpleVertex* vertArr = new SimpleVertex[3];
	vertArr[0].pos = { 1.0f,  1.0f, 0.0f };
	vertArr[0].color = { 1.0f, 0.0f, 0.0f };
	vertArr[1].pos = { -1.0f,  1.0f, 0.0f };
	vertArr[1].color = { 0.0f, 1.0f, 0.0f };
	vertArr[2].pos = { 0.0f, -1.0f, 0.0f }; 
	vertArr[2].color = { 0.0f, 0.0f, 1.0f };

	std::vector<uint32_t> indices = { 0, 1, 2 };

	//Staging buffer
	VulkanBuffer* stagingBuffer = new Zodiac::VulkanBuffer(m_device, sizeof(SimpleVertex), 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer->MapMemory();
	stagingBuffer->SetData(vertArr);
	stagingBuffer->UnmapMemory();

	//Device local buffer
	m_vertexBuffer = new Zodiac::VulkanBuffer(m_device, sizeof(SimpleVertex), 3, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VulkanBuffer* indexBuffer_staging = new Zodiac::VulkanBuffer(m_device, sizeof(uint32_t), 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	indexBuffer_staging->MapMemory();
	indexBuffer_staging->SetData(indices.data());
	indexBuffer_staging->UnmapMemory();

	m_indexBuffer = new Zodiac::VulkanBuffer(m_device, sizeof(uint32_t), 3, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer copyCommand = m_device->GetCommandBuffer(true);
	m_vertexBuffer->CopyFrom(copyCommand, stagingBuffer);
	m_indexBuffer->CopyFrom(copyCommand, indexBuffer_staging);
	m_device->FlushCommandBuffer(copyCommand, *m_device->GetGraphicsQueue(), m_device->GetGraphicsCommandPool()); //TODO: Might change this a bit

	delete stagingBuffer;
	delete indexBuffer_staging;
	delete[] vertArr;
}

void Zodiac::Renderer::PrepareUniformBuffers() {
	m_uniformBuffer = new VulkanBuffer(m_device, sizeof(uboVS), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	m_uniformBuffer->p_descriptor.buffer = m_uniformBuffer->GetBuffer();
	m_uniformBuffer->p_descriptor.offset = 0;
	m_uniformBuffer->p_descriptor.range = sizeof(uboVS);

	m_uniformBuffer->MapMemory();
	m_uniformBuffer->SetData(&uboVS);
	m_uniformBuffer->UnmapMemory();
}

void Zodiac::Renderer::SetupDescriptorSets() {
	// Binding 0: Uniform buffer (Vertex shader)
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = Initializers::DescriptorSetLayoutCreateInfo(1, layoutBinding);
	ErrorCheck(vkCreateDescriptorSetLayout(*m_device->GetDevice(), &layoutCreateInfo, nullptr, &m_descriptorSetLayout));

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = Initializers::PipelineLayoutCreateInfo(1, m_descriptorSetLayout);
	ErrorCheck(vkCreatePipelineLayout(*m_device->GetDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void Zodiac::Renderer::SetupDescriptorPool() {
	VkDescriptorPoolSize typeCounts[1];

	typeCounts[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	typeCounts[0].descriptorCount = 1;
	// For additional types you need to add new entries in the type count list
	// E.g. for two combined image samplers :
	// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// typeCounts[1].descriptorCount = 2;

	VkDescriptorPoolCreateInfo descriptorPoolInfo = Initializers::DescriptorPoolCreateInfo(typeCounts->descriptorCount, typeCounts, typeCounts->descriptorCount);
	ErrorCheck(vkCreateDescriptorPool(*m_device->GetDevice(), &descriptorPoolInfo, nullptr, &m_descriptorPool));
}

void Zodiac::Renderer::PrepareDescriptorSet() {
	VkDescriptorSetAllocateInfo descriptorSetAllocInfo = Initializers::DescriptorSetAllocateInfo(&m_descriptorPool, 1, &m_descriptorSetLayout);
	ErrorCheck(vkAllocateDescriptorSets(*m_device->GetDevice(), &descriptorSetAllocInfo, &m_descriptorSet));

	VkWriteDescriptorSet writeDescriptorSet = {};

	// Binding 0 : Uniform buffer
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = m_descriptorSet;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pBufferInfo = &m_uniformBuffer->p_descriptor;
	// Binds this uniform buffer to binding point 0
	writeDescriptorSet.dstBinding = 0;

	vkUpdateDescriptorSets(*m_device->GetDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

void Zodiac::Renderer::BuildCommandBuffers() {
	m_device->GetGraphicsCommand(m_drawCmdBuffers.data(), m_swapchain->GetImageCount()); //Allocates buffers

	VkCommandBufferBeginInfo commandBufferBeginInfo = Initializers::CommandBufferBeginInfo(0);
	VkRenderPassBeginInfo renderPassBeginInfo = Initializers::RenderPassBeginInfo(m_renderPass, m_swapchain->GetExtent2D(), m_clearValues, 2);

	for (int32_t i = 0; i < m_drawCmdBuffers.size(); i++) {
		renderPassBeginInfo.framebuffer = m_framebuffers[i];
		ErrorCheck(vkBeginCommandBuffer(m_drawCmdBuffers[i], &commandBufferBeginInfo));

		// This will clear the color and depth attachment
		vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			// Update dynamic viewport state
			VkViewport viewport = {};
			viewport.width = (float)m_swapchain->GetExtent2D().width;
			viewport.height = (float)m_swapchain->GetExtent2D().height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);
			// Update dynamic scissor state
			VkRect2D scissor = {};
			scissor.extent = m_swapchain->GetExtent2D();
			scissor.offset.x = 0;
			scissor.offset.y = 0;
			vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

			// Bind descriptor sets describing shader binding points
			vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);

			// Bind vertex buffer (contains position and colors)
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(m_drawCmdBuffers[i], 0, 1, &m_vertexBuffer->GetBuffer(), offsets);
			vkCmdBindIndexBuffer(m_drawCmdBuffers[i], m_indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

			vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
			vkCmdDrawIndexed(m_drawCmdBuffers[i], m_indexBuffer->GetCount(), 1, 0, 0, 1);
		vkCmdEndRenderPass(m_drawCmdBuffers[i]);

		ErrorCheck(vkEndCommandBuffer(m_drawCmdBuffers[i]));
	}
}

void Zodiac::Renderer::AllocateCommandBuffers() {
	m_device->GetGraphicsCommand(m_drawCmdBuffers.data(), m_swapchain->GetImageCount()); //Allocates buffers
}

void Zodiac::Renderer::RecordCommandBuffer(int32_t index, bool secondBarrier) {
	vkResetCommandBuffer(m_drawCmdBuffers[index], 0);

	VkCommandBufferBeginInfo commandBufferBeginInfo = Initializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT); //Valid for re-recording the command buffer every frame
	VkRenderPassBeginInfo renderPassBeginInfo = Initializers::RenderPassBeginInfo(m_renderPass, m_swapchain->GetExtent2D(), m_clearValues, 2);

	renderPassBeginInfo.framebuffer = m_framebuffers[index];
	ErrorCheck(vkBeginCommandBuffer(m_drawCmdBuffers[index], &commandBufferBeginInfo));

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_swapchain->GetImage(index);
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	vkCmdPipelineBarrier(m_drawCmdBuffers[index], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	vkCmdBeginRenderPass(m_drawCmdBuffers[index], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Update dynamic viewport state
	VkViewport viewport = {};
	viewport.width = (float)m_swapchain->GetExtent2D().width;
	viewport.height = (float)m_swapchain->GetExtent2D().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(m_drawCmdBuffers[index], 0, 1, &viewport);
	// Update dynamic scissor state
	VkRect2D scissor = {};
	scissor.extent = m_swapchain->GetExtent2D();
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	vkCmdSetScissor(m_drawCmdBuffers[index], 0, 1, &scissor);

	// Bind descriptor sets describing shader binding points
	vkCmdBindDescriptorSets(m_drawCmdBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);

	// Bind vertex buffer (contains position and colors)
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(m_drawCmdBuffers[index], 0, 1, &m_vertexBuffer->GetBuffer(), offsets);
	vkCmdBindIndexBuffer(m_drawCmdBuffers[index], m_indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindPipeline(m_drawCmdBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
	vkCmdDrawIndexed(m_drawCmdBuffers[index], m_indexBuffer->GetCount(), 1, 0, 0, 1);

	vkCmdEndRenderPass(m_drawCmdBuffers[index]);

	//This barrier should only be used without imgui, so it's disabled for now
	if (secondBarrier) {
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_swapchain->GetImage(index);
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		vkCmdPipelineBarrier(m_drawCmdBuffers[index], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	ErrorCheck(vkEndCommandBuffer(m_drawCmdBuffers[index]));
}

void Zodiac::Renderer::RecreateSwapChain()
{
	while (m_window->GetWidth() == 0 || m_window->GetHeight() == 0) {
		m_window->WaitEvents();
	}

	vkDeviceWaitIdle(*m_device->GetDevice());
	CleanupFramebuffers();
	CleanupSyncObjects();
	m_surface->QuerySurfaceDetails(m_device->GetPhysicalDevice());
	m_swapchain->Recreate(m_surface->GetSurfaceDetails(), m_surface->GetSurface(), m_settings, m_device);

	SetupFramebuffers();
	CreateSyncObjects();
}

void Zodiac::Renderer::CleanupFramebuffers()
{
	for (size_t i = 0; i < m_framebuffers.size(); i++) {
		vkDestroyFramebuffer(*m_device->GetDevice(), m_framebuffers[i], nullptr);
	}
	m_framebuffers.clear();
}

void Zodiac::Renderer::CreateSyncObjects()
{
	m_presentSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_renderCompleteSemaphores.resize(m_swapchain->GetImageCount());
	m_drawCmdBuffers.resize(m_swapchain->GetImageCount());
	m_waitFences.resize(MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		m_presentSemaphores[i] = new VulkanSemaphore(m_device);
		m_waitFences[i] = new VulkanFence(m_device);
	}
	for (size_t i = 0; i < m_swapchain->GetImageCount(); i++) {
		m_renderCompleteSemaphores[i] = new VulkanSemaphore(m_device);
	}
	m_imagesInFlight.resize(m_drawCmdBuffers.size(), VK_NULL_HANDLE);
}

void Zodiac::Renderer::CleanupSyncObjects()
{
	for (size_t i = 0; i < m_swapchain->GetImageCount(); i++) {
		delete m_renderCompleteSemaphores[i];
	}
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		delete m_presentSemaphores[i];
		delete m_waitFences[i];
	}
	//for (size_t i = 0; i < s_imagesInFlight.size(); i++) {
	//delete s_imagesInFlight[i];
	//}
	m_renderCompleteSemaphores.clear();
	m_presentSemaphores.clear();
	m_waitFences.clear();
	m_imagesInFlight.clear();
}

void Zodiac::Renderer::UpdateUniformBuffers(uint32_t currentImage, float dt, Camera* mainCamera)
{
	m_uniformBuffer->p_descriptor.range = sizeof(uboVS);

	testVal += 0.5f * dt;

	uboVS.projectionMatrix = mainCamera->GetProjection();
	uboVS.viewMatrix = mainCamera->GetView();

	uboVS.modelMatrix = glm::mat4(1.0f);
	uboVS.modelMatrix = glm::rotate(uboVS.modelMatrix, glm::vec3(testVal).x, glm::vec3(1.0f, 0.0f, 0.0f)); //TODO: Add changable parameters
	uboVS.modelMatrix = glm::rotate(uboVS.modelMatrix, glm::vec3().y, glm::vec3(0.0f, 1.0f, 0.0f));
	uboVS.modelMatrix = glm::rotate(uboVS.modelMatrix, glm::vec3().z, glm::vec3(0.0f, 0.0f, 1.0f));

	m_uniformBuffer->MapMemory();
	m_uniformBuffer->SetData(&uboVS); //Since the buffer has been passed a pointer at creation, it should technically be okay to do this without passing anything else
	m_uniformBuffer->UnmapMemory();
}

void Zodiac::Renderer::Shutdown() {
	if (m_showGui) {
		m_imgui->Shutdown();
	}

	CleanupFramebuffers();
	CleanupSyncObjects();

	vkDestroyPipelineCache(*m_device->GetDevice(), m_pipelineCache, nullptr);
	vkDestroyDescriptorSetLayout(*m_device->GetDevice(), m_descriptorSetLayout, nullptr);
	vkDestroyPipelineLayout(*m_device->GetDevice(), m_pipelineLayout, nullptr);
	vkDestroyRenderPass(*m_device->GetDevice(), m_renderPass, nullptr);
	vkDestroyPipeline(*m_device->GetDevice(), m_pipeline, nullptr);
	vkDestroyDescriptorPool(*m_device->GetDevice(), m_descriptorPool, nullptr);

	delete m_swapchain;

	//Note to self: Keep an eye on these so that they're destroyed at the right time
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_uniformBuffer;
}

void Zodiac::Renderer::ToggleImGui()
{
	m_showGui = !m_showGui;
}

void Zodiac::Renderer::SetFramebufferResized(bool resized)
{
	m_swapchainDirty = resized;
}

Zodiac::Settings& Zodiac::Renderer::GetSettings()
{
	return m_settings;
}

void Zodiac::Renderer::SetClearColor(const glm::vec4 color) {
	m_clearValues[0].color = { color.r, color.g, color.b, color.a };
}

void Zodiac::Renderer::SetSwapchainDirty()
{
	m_swapchainDirty = true;
}
