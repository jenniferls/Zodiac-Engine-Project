#pragma once
#include <vulkan/vulkan.h>

namespace Zodiac {
	struct VulkanSwapChainBuffer {
		VkImage image;
		VkImageView view;
	};
}