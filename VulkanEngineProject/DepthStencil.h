#pragma once
#include <vulkan/vulkan.h>

namespace Zodiac {
	struct DepthStencil {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	};
}