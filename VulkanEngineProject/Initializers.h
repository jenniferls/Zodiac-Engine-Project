#pragma once
#include <vector>
#include <vulkan\vulkan.h>

#include "Validation.h"
#include "VulkanConfiguration.h"

namespace Zodiac { namespace Initializers {
		VkApplicationInfo ApplicationInfo(VulkanConfiguration& config);
		VkInstanceCreateInfo InstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& layers, std::vector<const char*>& extensions);
		VkDeviceCreateInfo DeviceCreateInfo(std::vector<VkDeviceQueueCreateInfo>& queue_create_info, VkPhysicalDeviceFeatures& physical_device_features);
		VkDeviceQueueCreateInfo DeviceQueueCreate(uint32_t queue_family_index, float& priority);
		VkCommandPoolCreateInfo CommandPoolCreateinfo(uint32_t queue_family_index, VkCommandPoolCreateFlags flags = 0);
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count);
		VkBufferCreateInfo BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage_flags);
		VkMemoryAllocateInfo MemoryAllocateInfo(VkDeviceSize size, uint32_t memory_type_index);

		VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo();

		VkSubmitInfo SubmitInfo();
		VkRenderPassBeginInfo RenderPassBeginInfo();
	}
}