#include <Zodiacpch.h>
#include "VulkanFramebuffer.h"
#include "Initializers.h"
#include "Validation.h"

Zodiac::VulkanFramebuffer::VulkanFramebuffer(VkDevice* device, VkRenderPass& renderPass, VulkanSwapChainBuffer& swapchainBuffer, DepthStencil& depthStencil, uint32_t width, uint32_t height) {
	m_device = device;
	m_attachments[0] = swapchainBuffer.view;	// Color attachment is the view of the swapchain image			
	m_attachments[1] = depthStencil.view;		// Depth/Stencil attachment is the same for all frame buffers

	VkFramebufferCreateInfo framebufferInfo = Initializers::FramebufferCreateInfo(renderPass, m_attachments, width, height);
	ErrorCheck(vkCreateFramebuffer(*device, &framebufferInfo, nullptr, &m_framebuffer));
}

Zodiac::VulkanFramebuffer::~VulkanFramebuffer() {
	vkDestroyFramebuffer(*m_device, m_framebuffer, nullptr);
}

VkFramebuffer& Zodiac::VulkanFramebuffer::GetFramebuffer() {
	return m_framebuffer;
}
