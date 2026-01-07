#include "Zodiacpch.h"
#include "ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "Defines.h"
#include "Validation.h"
#include "Renderer.h"
#include "Initializers.h"
#include "PerspectiveCamera.h"

Zodiac::VulkanDevice* Zodiac::ImGuiLayer::s_device;
VkDescriptorPool Zodiac::ImGuiLayer::s_descriptorPool;
std::vector<VkCommandBuffer> Zodiac::ImGuiLayer::s_command_buffers;


Zodiac::ImGuiLayer::ImGuiLayer() {
}

Zodiac::ImGuiLayer::~ImGuiLayer() {

}

Zodiac::ImGuiLayer* Zodiac::ImGuiLayer::Create() {
	return new ImGuiLayer();
}

void Zodiac::ImGuiLayer::Shutdown()
{
	s_device->FreeCommandBuffers(s_command_buffers.size(), s_command_buffers.data());
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	vkDestroyDescriptorPool(*s_device->GetDevice(), s_descriptorPool, NULL);
}

void Zodiac::ImGuiLayer::UpdateGUI(Camera* camera) {
	ImGuiIO& io = ImGui::GetIO();

	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	Renderer& renderer = Renderer::Get();
	Settings& settings = renderer.GetSettings();
	PerspectiveCamera* cam = static_cast<PerspectiveCamera*>(camera);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (m_showDemoWindow) {
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float fov = cam->GetFoV();
		static int counter = 0;
		bool vsync = settings.vsync;
		glm::vec4 clear_color = m_clearColor;

		ImGui::Begin("Debug window");

		ImGui::Text("Welcome to Zodiac Engine!");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &m_showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &m_showAnotherWindow);
		ImGui::Checkbox("VSync", &settings.vsync);
		if (vsync != settings.vsync) {
			renderer.SetSwapchainDirty();
		}

		ImGui::SliderFloat("FoV", &fov, 0.0f, 180.0f);
		if (fov != cam->GetFoV()) {
			cam->SetFoV(fov);
		}
		ImGui::ColorEdit3("clear color", (float*)&m_clearColor); // Edit 3 floats representing a color
		if (clear_color != m_clearColor) {
			renderer.SetClearColor(m_clearColor);
		}

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Models: %d", renderer.m_scene.GetModelCount());
		ImGui::Text("Meshes: %d", renderer.m_scene.GetSceneMeshCount());
		ImGui::Text("Triangles: %d", renderer.m_scene.GetSceneVertexCount() / 3);
		ImGui::Text("Vertices: %d", renderer.m_scene.GetSceneVertexCount());

		ImGui::End();
	}

	// 3. Show another simple window.
	if (m_showAnotherWindow)
	{
		ImGui::ShowMetricsWindow(&m_showAnotherWindow);
	}

	// Rendering
	ImGui::Render();
}

// Must be called after the ImGUI frame was prepared on the application side!
VkCommandBuffer Zodiac::ImGuiLayer::PrepareCommandBuffer(int image) {
	Renderer& renderer = Renderer::Get();

	s_device->BeginCommandBuffer(s_command_buffers[image], VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	
	renderer.BeginDynamicRendering(s_command_buffers[image], image, NULL, NULL);

	ImDrawData* pDrawData = ImGui::GetDrawData();
	ImGui_ImplVulkan_RenderDrawData(pDrawData, s_command_buffers[image]);

	vkCmdEndRendering(s_command_buffers[image]);

	//Needs some sort of wrapper
	VkImageLayout oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkImageLayout newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkFormat format = renderer.m_swapchain->GetSurfaceFormat().format;
	{
		VkImageMemoryBarrier barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.pNext = NULL,
		.srcAccessMask = 0,
		.dstAccessMask = 0,
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = renderer.m_swapchain->GetImage(image),
		.subresourceRange = VkImageSubresourceRange {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
		};

		VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
		VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
			(format == VK_FORMAT_D16_UNORM) ||
			(format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
			(format == VK_FORMAT_D32_SFLOAT) ||
			(format == VK_FORMAT_S8_UINT) ||
			(format == VK_FORMAT_D16_UNORM_S8_UINT) ||
			(format == VK_FORMAT_D24_UNORM_S8_UINT))
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (renderer.m_swapchain->SurfaceHasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} /* Convert back from read-only to updateable */
		else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} /* Convert from updateable texture to shader read-only */
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert depth texture from undefined state to depth-stencil buffer */
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		} /* Wait for render pass to complete */
		else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = 0; // VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = 0;
			/*
					sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			///		destinationStage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
					destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			*/
			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert back from read-only to color attachment */
		else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		} /* Convert from updateable texture to shader read-only */
		else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert back from read-only to depth attachment */
		else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		} /* Convert from updateable depth texture to shader read-only */
		else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = 0;

			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}
		else {
			printf("Unknown barrier case\n");
			exit(1);
		}

		vkCmdPipelineBarrier(s_command_buffers[image], sourceStage, destinationStage,
			0, 0, NULL, 0, NULL, 1, &barrier);
	}

	vkEndCommandBuffer(s_command_buffers[image]);

	return s_command_buffers[image];
}

bool Zodiac::ImGuiLayer::Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance) {
	s_device = device;
	Renderer& renderer = Renderer::Get();

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.DisplaySize.x = (float)width;
	io.DisplaySize.y = (float)height;
	ImGui::StyleColorsDark();
	ImGui::GetStyle().FontScaleMain = 1.5f;

	std::cout << "ImGui context successfully created." << std::endl;

	ImGui_ImplGlfw_InitForVulkan(window, true);

	//Has to be done here because of initialization order
	SetupDescriptorPool();

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.ApiVersion = instance->GetInstanceVersion();
	init_info.Instance = instance->GetInstance();
	init_info.PhysicalDevice = s_device->GetPhysicalDevice()->GetPhysicalDevice();
	init_info.Device = *s_device->GetDevice();
	init_info.QueueFamily = s_device->GetPhysicalDevice()->GetFamilyIndices().graphics_indices;
	init_info.Queue = *s_device->GetGraphicsQueue();
	init_info.DescriptorPool = s_descriptorPool; //This is a imgui-specific descriptor pool
	init_info.RenderPass = VK_NULL_HANDLE; //For dynamic rendering
	init_info.MinImageCount = m_minImageCount;
	init_info.ImageCount = renderer.m_swapchain->GetImageCount();
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.PipelineCache = VK_NULL_HANDLE; //Should work for now
	init_info.Subpass = 0;
	init_info.UseDynamicRendering = VK_TRUE;
	//dynamic rendering parameters for imgui to use
	init_info.PipelineRenderingCreateInfo = {};
	init_info.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	init_info.PipelineRenderingCreateInfo.pNext = NULL;
	init_info.PipelineRenderingCreateInfo.viewMask = 0;
	init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &renderer.m_swapchain->GetSurfaceFormat().format;
	init_info.PipelineRenderingCreateInfo.depthAttachmentFormat = renderer.m_swapchain->GetDepthFormat();
	init_info.PipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED; // No stencil attachment
	init_info.Allocator = NULL;
	init_info.CheckVkResultFn = ErrorCheck;

	ImGui_ImplVulkan_Init(&init_info);

	//VkCommandPool command_pool = device->GetGraphicsCommandPool();
	s_command_buffers.resize(renderer.m_swapchain->GetImageCount());
	device->CreateCommandBuffers(renderer.m_swapchain->GetImageCount(), s_command_buffers.data());

	//err = vkResetCommandPool(device->GetDevice(), command_pool, 0);
	//check_vk_result(err);

	//ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

	// add the destroy the imgui created structures
	//_mainDeletionQueue.push_function([=]() {
	//	ImGui_ImplVulkan_Shutdown();
	//	vkDestroyDescriptorPool(*device->GetDevice(), imguiPool, nullptr);
	//	});

	//device->FlushCommandBuffer(command_buffer,*device->GetGraphicsQueue(), command_pool );

	//ImGui_ImplVulkan_DestroyFontUploadObjects();

	m_initialized = true;
	return true;
}

void Zodiac::ImGuiLayer::SetupRenderPass()
{
	//// Color attachment
	//std::vector<VkAttachmentDescription> attachments(1);
	//attachments[0].format = Zodiac::Renderer::s_swapchain->GetSurfaceFormat().format;
	//attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	//attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	//attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//// Setup attachment references
	//VkAttachmentReference colorReference = {};
	//colorReference.attachment = 0;													// Attachment 0 is color
	//colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;				// Attachment layout used as color during the subpass

	//// Setup a single subpass reference
	//std::vector<VkSubpassDescription> subpassDescriptions(1);
	//subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	//subpassDescriptions[0].colorAttachmentCount = 1;									// Subpass uses one color attachment
	//subpassDescriptions[0].pColorAttachments = &colorReference;							// Reference to the color attachment in slot 0

	//std::vector<VkSubpassDependency> dependencies(1);
	//dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	//dependencies[0].dstSubpass = 0;
	//dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//dependencies[0].srcAccessMask = 0;
	//dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//// Create the actual renderpass
	//VkRenderPassCreateInfo renderPassInfo = Initializers::RenderPassCreateInfo(attachments, subpassDescriptions, dependencies);
	//ErrorCheck(vkCreateRenderPass(*s_device->GetDevice(), &renderPassInfo, nullptr, &s_renderPass));
}

void Zodiac::ImGuiLayer::SetupDescriptorPool()
{
	//Allocating space for quite a lot of stuff, but we could still theoretically run out (although we shouldn't!)
	int32_t descriptorCount = 1000;
	VkDescriptorPoolSize poolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descriptorCount }
	};

	VkDescriptorPoolCreateInfo descriptorPoolInfo = Initializers::DescriptorPoolCreateInfo(IM_ARRAYSIZE(poolSizes), poolSizes, descriptorCount * IM_ARRAYSIZE(poolSizes), VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
	ErrorCheck(vkCreateDescriptorPool(*s_device->GetDevice(), &descriptorPoolInfo, nullptr, &s_descriptorPool));
	std::cout << "Imgui descriptor pool successfully setup." << std::endl;
}
