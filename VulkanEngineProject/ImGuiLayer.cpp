#include "Zodiacpch.h"
#include "ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "Defines.h"
#include "Validation.h"
#include "Renderer.h"
#include "Initializers.h"

Zodiac::VulkanDevice* Zodiac::ImGuiLayer::s_device;
//VkRenderPass Zodiac::ImGuiLayer::s_renderPass;
VkDescriptorPool Zodiac::ImGuiLayer::s_descriptorPool;


Zodiac::ImGuiLayer::ImGuiLayer() {
}

Zodiac::ImGuiLayer::~ImGuiLayer() {

}

Zodiac::ImGuiLayer* Zodiac::ImGuiLayer::Create() {
	return new ImGuiLayer();
}

void Zodiac::ImGuiLayer::Shutdown()
{
	vkDestroyDescriptorPool(*s_device->GetDevice(), s_descriptorPool, nullptr);
	//vkDestroyRenderPass(*s_device->GetDevice(), s_renderPass, nullptr);
	//ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Zodiac::ImGuiLayer::Render(Window* window, VulkanInstance* instance)
{
	//// Resize swap chain?
	//if (m_swapchainRebuild)
	//{
		int width, height;
		glfwGetFramebufferSize((GLFWwindow*)window->GetNativeWindow(), &width, &height);
	//	if (width > 0 && height > 0)
	//	{
	//		ImGui_ImplVulkan_SetMinImageCount(m_minImageCount);
	//		//ImGui_ImplVulkanH_CreateOrResizeWindow(instance->GetInstance(), s_device->GetPhysicalDevice()->GetPhysicalDevice(), *s_device->GetDevice(), &s_mainWindowData, s_device->GetPhysicalDevice()->GetFamilyIndices().graphics_indices, nullptr, width, height, m_minImageCount);
	//		//s_mainWindowData.FrameIndex = 0;
	//		m_swapchainRebuild = false;
	//	}
	//}

	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (m_showDemoWindow)
		ImGui::ShowDemoWindow(&m_showDemoWindow);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &m_showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &m_showAnotherWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&m_clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (m_showAnotherWindow)
	{
		ImGui::Begin("Another Window", &m_showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			m_showAnotherWindow = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
	if (!is_minimized)
	{
		//// FrameRender(wd, draw_data); //// From main.cpp in imgui examples for glfw and vulkan

		// This part should be done by Renderer::Draw()

		//VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
		//VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		//err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
		//if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		//{
		//	g_SwapChainRebuild = true;
		//	return;
		//}
		//check_vk_result(err);

		//mGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
		//{
		//	err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
		//	check_vk_result(err);

		//	err = vkResetFences(g_Device, 1, &fd->Fence);
		//	check_vk_result(err);
		//}

		//

		// This part is done in Renderer::BuildCommandBuffers() during initialization

		//{
		//	err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
		//	check_vk_result(err);
		//	VkCommandBufferBeginInfo info = {};
		//	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		//	err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		//	check_vk_result(err);
		//}
		//{
		//	VkRenderPassBeginInfo info = {};
		//	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//	info.renderPass = wd->RenderPass;
		//	info.framebuffer = fd->Framebuffer;
		//	info.renderArea.extent.width = wd->Width;
		//	info.renderArea.extent.height = wd->Height;
		//	info.clearValueCount = 1;
		//	info.pClearValues = &wd->ClearValue;
		//	vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		//}

		//

		//// Record dear imgui primitives into command buffer ////
		//ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
		//// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		//int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
		//int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
		//if (fb_width <= 0 || fb_height <= 0)
		//	return;

		//ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
		//ImGui_ImplVulkan_InitInfo* v = &bd->VulkanInitInfo;
		//VkPipeline pipeline;
		//if (pipeline == VK_NULL_HANDLE)
		//	pipeline = bd->Pipeline;

		//// Allocate array to store enough vertex/index buffers
		//ImGui_ImplVulkanH_WindowRenderBuffers* wrb = &bd->MainWindowRenderBuffers;
		//if (wrb->FrameRenderBuffers == nullptr)
		//{
		//	wrb->Index = 0;
		//	wrb->Count = v->ImageCount;
		//	wrb->FrameRenderBuffers = (ImGui_ImplVulkanH_FrameRenderBuffers*)IM_ALLOC(sizeof(ImGui_ImplVulkanH_FrameRenderBuffers) * wrb->Count);
		//	memset(wrb->FrameRenderBuffers, 0, sizeof(ImGui_ImplVulkanH_FrameRenderBuffers) * wrb->Count);
		//}
		//IM_ASSERT(wrb->Count == v->ImageCount);
		//wrb->Index = (wrb->Index + 1) % wrb->Count;
		//ImGui_ImplVulkanH_FrameRenderBuffers* rb = &wrb->FrameRenderBuffers[wrb->Index];

		//if (draw_data->TotalVtxCount > 0)
		//{
		//	// Create or resize the vertex/index buffers
		//	size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
		//	size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
		//	if (rb->VertexBuffer == VK_NULL_HANDLE || rb->VertexBufferSize < vertex_size)
		//		CreateOrResizeBuffer(rb->VertexBuffer, rb->VertexBufferMemory, rb->VertexBufferSize, vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		//	if (rb->IndexBuffer == VK_NULL_HANDLE || rb->IndexBufferSize < index_size)
		//		CreateOrResizeBuffer(rb->IndexBuffer, rb->IndexBufferMemory, rb->IndexBufferSize, index_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

		//	// Upload vertex/index data into a single contiguous GPU buffer
		//	ImDrawVert* vtx_dst = nullptr;
		//	ImDrawIdx* idx_dst = nullptr;
		//	VkResult err = vkMapMemory(v->Device, rb->VertexBufferMemory, 0, rb->VertexBufferSize, 0, (void**)(&vtx_dst));
		//	check_vk_result(err);
		//	err = vkMapMemory(v->Device, rb->IndexBufferMemory, 0, rb->IndexBufferSize, 0, (void**)(&idx_dst));
		//	check_vk_result(err);
		//	for (int n = 0; n < draw_data->CmdListsCount; n++)
		//	{
		//		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		//		memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
		//		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		//		vtx_dst += cmd_list->VtxBuffer.Size;
		//		idx_dst += cmd_list->IdxBuffer.Size;
		//	}
		//	VkMappedMemoryRange range[2] = {};
		//	range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		//	range[0].memory = rb->VertexBufferMemory;
		//	range[0].size = VK_WHOLE_SIZE;
		//	range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		//	range[1].memory = rb->IndexBufferMemory;
		//	range[1].size = VK_WHOLE_SIZE;
		//	err = vkFlushMappedMemoryRanges(v->Device, 2, range);
		//	check_vk_result(err);
		//	vkUnmapMemory(v->Device, rb->VertexBufferMemory);
		//	vkUnmapMemory(v->Device, rb->IndexBufferMemory);
		//}

		//// Setup desired Vulkan state
		//ImGui_ImplVulkan_SetupRenderState(draw_data, pipeline, command_buffer, rb, fb_width, fb_height);

		//// Will project scissor/clipping rectangles into framebuffer space
		//ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
		//ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

		//// Render command lists
		//// (Because we merged all buffers into a single one, we maintain our own offset into them)
		//int global_vtx_offset = 0;
		//int global_idx_offset = 0;
		//for (int n = 0; n < draw_data->CmdListsCount; n++)
		//{
		//	const ImDrawList* cmd_list = draw_data->CmdLists[n];
		//	for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		//	{
		//		const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
		//		if (pcmd->UserCallback != nullptr)
		//		{
		//			// User callback, registered via ImDrawList::AddCallback()
		//			// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
		//			if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
		//				ImGui_ImplVulkan_SetupRenderState(draw_data, pipeline, command_buffer, rb, fb_width, fb_height);
		//			else
		//				pcmd->UserCallback(cmd_list, pcmd);
		//		}
		//		else
		//		{
		//			// Project scissor/clipping rectangles into framebuffer space
		//			ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
		//			ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

		//			// Clamp to viewport as vkCmdSetScissor() won't accept values that are off bounds
		//			if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
		//			if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
		//			if (clip_max.x > fb_width) { clip_max.x = (float)fb_width; }
		//			if (clip_max.y > fb_height) { clip_max.y = (float)fb_height; }
		//			if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
		//				continue;

		//			// Apply scissor/clipping rectangle
		//			VkRect2D scissor;
		//			scissor.offset.x = (int32_t)(clip_min.x);
		//			scissor.offset.y = (int32_t)(clip_min.y);
		//			scissor.extent.width = (uint32_t)(clip_max.x - clip_min.x);
		//			scissor.extent.height = (uint32_t)(clip_max.y - clip_min.y);
		//			vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		//			// Bind DescriptorSet with font or user texture
		//			VkDescriptorSet desc_set[1] = { (VkDescriptorSet)pcmd->TextureId };
		//			if (sizeof(ImTextureID) < sizeof(ImU64))
		//			{
		//				// We don't support texture switches if ImTextureID hasn't been redefined to be 64-bit. Do a flaky check that other textures haven't been used.
		//				IM_ASSERT(pcmd->TextureId == (ImTextureID)bd->FontDescriptorSet);
		//				desc_set[0] = bd->FontDescriptorSet;
		//			}
		//			vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bd->PipelineLayout, 0, 1, desc_set, 0, nullptr);

		//			// Draw
		//			vkCmdDrawIndexed(command_buffer, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
		//		}
		//	}
		//	global_idx_offset += cmd_list->IdxBuffer.Size;
		//	global_vtx_offset += cmd_list->VtxBuffer.Size;
		//}

		//// Note: at this point both vkCmdSetViewport() and vkCmdSetScissor() have been called.
		//// Our last values will leak into user/application rendering IF:
		//// - Your app uses a pipeline with VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR dynamic state
		//// - And you forgot to call vkCmdSetViewport() and vkCmdSetScissor() yourself to explicitly set that state.
		//// If you use VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR you are responsible for setting the values before rendering.
		//// In theory we should aim to backup/restore those values but I am not sure this is possible.
		//// We perform a call to vkCmdSetScissor() to set back a full viewport which is likely to fix things for 99% users but technically this is not perfect. (See github #4644)
		//VkRect2D scissor = { { 0, 0 }, { (uint32_t)fb_width, (uint32_t)fb_height } };
		//vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		//// Submit command buffer
		//vkCmdEndRenderPass(fd->CommandBuffer);
		//{
		//	VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//	VkSubmitInfo info = {};
		//	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//	info.waitSemaphoreCount = 1;
		//	info.pWaitSemaphores = &image_acquired_semaphore;
		//	info.pWaitDstStageMask = &wait_stage;
		//	info.commandBufferCount = 1;
		//	info.pCommandBuffers = &fd->CommandBuffer;
		//	info.signalSemaphoreCount = 1;
		//	info.pSignalSemaphores = &render_complete_semaphore;

		//	err = vkEndCommandBuffer(fd->CommandBuffer);
		//	check_vk_result(err);
		//	err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
		//	check_vk_result(err);
		//}

		//// Submit command buffer
		//vkCmdEndRenderPass(fd->CommandBuffer);
		//{
		//	VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//	VkSubmitInfo info = {};
		//	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//	info.waitSemaphoreCount = 1;
		//	info.pWaitSemaphores = &image_acquired_semaphore;
		//	info.pWaitDstStageMask = &wait_stage;
		//	info.commandBufferCount = 1;
		//	info.pCommandBuffers = &fd->CommandBuffer;
		//	info.signalSemaphoreCount = 1;
		//	info.pSignalSemaphores = &render_complete_semaphore;

		//	err = vkEndCommandBuffer(fd->CommandBuffer);
		//	check_vk_result(err);
		//	err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
		//	check_vk_result(err);
		//}

		////////

		//// FramePresent(window); ////
		//if (g_SwapChainRebuild)
		//	return;
		//VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		//VkPresentInfoKHR info = {};
		//info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		//info.waitSemaphoreCount = 1;
		//info.pWaitSemaphores = &render_complete_semaphore;
		//info.swapchainCount = 1;
		//info.pSwapchains = &wd->Swapchain;
		//info.pImageIndices = &wd->FrameIndex;
		//VkResult err = vkQueuePresentKHR(g_Queue, &info);
		//if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
		//{
		//	g_SwapChainRebuild = true;
		//	return;
		//}
		//check_vk_result(err);
		//wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores

		//Renderer::Draw();
	}
}

bool Zodiac::ImGuiLayer::Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance) {
	s_device = device;

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
	//SetupRenderPass();

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
	init_info.ImageCount = Zodiac::Renderer::s_swapchain->GetImageCount();
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
	init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &Zodiac::Renderer::s_swapchain->GetSurfaceFormat().format;
	init_info.PipelineRenderingCreateInfo.depthAttachmentFormat = Zodiac::Renderer::s_swapchain->GetDepthFormat();
	init_info.PipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED; // No stencil attachment
	init_info.Allocator = NULL;
	init_info.CheckVkResultFn = ErrorCheck;

	ImGui_ImplVulkan_Init(&init_info);

	VkCommandPool command_pool = device->GetGraphicsCommandPool();
	VkCommandBuffer command_buffer = device->GetCommandBuffer(true);

	//err = vkResetCommandPool(device->GetDevice(), command_pool, 0);
	//check_vk_result(err);

	//ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

	// add the destroy the imgui created structures
	//_mainDeletionQueue.push_function([=]() {
	//	ImGui_ImplVulkan_Shutdown();
	//	vkDestroyDescriptorPool(*device->GetDevice(), imguiPool, nullptr);
	//	});

	device->FlushCommandBuffer(command_buffer,*device->GetGraphicsQueue(), command_pool );

	//ImGui_ImplVulkan_DestroyFontUploadObjects();

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
