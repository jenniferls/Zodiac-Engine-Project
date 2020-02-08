#pragma once
#include <vulkan/vulkan.h>
#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanFence {
	public:
		VulkanFence(VulkanDevice* device);
		~VulkanFence();

	private:
		VkDevice* m_device;
		VkFence m_fence;
	};
}