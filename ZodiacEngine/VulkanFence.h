#pragma once
#include <vulkan/vulkan.h>
#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanFence {
	public:
		VulkanFence(VulkanDevice* device);
		~VulkanFence();

		VkFence p_fence;

	private:
		VkDevice* m_device;
	};
}