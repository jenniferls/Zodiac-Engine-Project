#pragma once
#include <vulkan/vulkan.h>
#include "VulkanSwapChainBuffer.h"
#include "DepthStencil.h"

namespace Zodiac {
	class VulkanFramebuffer {
	public:
		VulkanFramebuffer(VkDevice* device, VkRenderPass& renderPass, VulkanSwapChainBuffer& swapchainBuffer, DepthStencil& depthStencil, uint32_t width, uint32_t height);
		~VulkanFramebuffer();

		VkFramebuffer& GetFramebuffer();

	private:
		VkDevice* m_device;
		VkFramebuffer m_framebuffer;
		std::vector<VkImageView> m_attachments;
	};
}