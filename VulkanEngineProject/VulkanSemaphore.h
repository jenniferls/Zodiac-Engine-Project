#pragma once
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanSemaphore {
	public:
		VulkanSemaphore(VulkanDevice* device);
		~VulkanSemaphore();

		VkSemaphore& GetSemaphore();

	private:
		VkSemaphore m_semaphore;
		VulkanDevice* m_device;
	};
}