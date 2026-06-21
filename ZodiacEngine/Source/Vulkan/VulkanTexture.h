#pragma once
#include <vulkan/vulkan.h>

#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanTexture {
	public:
        VulkanTexture();
		~VulkanTexture();
        
        void Create(const void* pPixels, uint32_t imageWidth, uint32_t imageHeight, bool isCubemap, VulkanDevice* device);

	private:
        VkImage m_image = VK_NULL_HANDLE;
        VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        VkSampler m_sampler = VK_NULL_HANDLE;
        VkImageLayout m_imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        
        bool m_isRGB = false;
	};
}
