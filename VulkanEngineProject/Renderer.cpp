#include <Zodiacpch.h>
#include "Renderer.h"

Zodiac::Renderer* Zodiac::Renderer::s_instance = nullptr;
Zodiac::VulkanDevice* Zodiac::Renderer::s_device;
Zodiac::VulkanSurface* Zodiac::Renderer::s_surface;
Zodiac::Settings Zodiac::Renderer::s_settings;

Zodiac::Renderer::Renderer() {

}

Zodiac::Renderer::~Renderer() {
	delete m_swapchain;
}

void Zodiac::Renderer::Init(VulkanDevice* device, Settings settings, VulkanSurface* surface) {
	s_device = device;
	s_settings = settings;
	s_surface = surface;
}

Zodiac::Renderer& Zodiac::Renderer::Get() {
	return *s_instance;
}

void Zodiac::Renderer::Init() {
	m_swapchain = new Zodiac::VulkanSwapchain(s_device, s_surface->GetSurfaceDetails(), s_surface->GetSurface(), s_settings);

	//Tests
	m_drawCmdBuffers.resize(m_swapchain->GetImageCount());
	s_device->GetGraphicsCommand(m_drawCmdBuffers.data(), m_swapchain->GetImageCount());

	s_device->FreeGraphicsCommand(m_drawCmdBuffers.data(), m_swapchain->GetImageCount());
	///////
}

void Zodiac::Renderer::SetupRenderPass() {
	// Descriptors for the attachments used by this renderpass
	std::array<VkAttachmentDescription, 2> attachments = {};

	// Color attachment
	attachments[0].format = m_swapchain->GetSurfaceFormat().format;					// Use the color format selected by the swapchain
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;									// We don't use multi sampling in this example
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;							// Clear this attachment at the start of the render pass
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;							// Keep it's contents after the render pass is finished (for displaying it)
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;					// We don't use stencil, so don't care for load
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;				// Same for store
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;						// Layout at render pass start. Initial doesn't matter, so we use undefined
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;					// Layout to which the attachment is transitioned when the render pass is finished
																					// As we want to present the color buffer to the swapchain, we transition to PRESENT_KHR
}
