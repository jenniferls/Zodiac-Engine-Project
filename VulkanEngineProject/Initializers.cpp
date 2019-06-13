#include "Initializers.h"

VkApplicationInfo Zodiac::Initializers::ApplicationInfo(VulkanConfiguration& config) {
	VkApplicationInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	info.pApplicationName = config.app_name;
	info.applicationVersion = config.app_version;
	info.pEngineName = config.engine_name;
	info.engineVersion = config.engine_version;
	info.apiVersion = config.api_version;
	return info;
}

VkInstanceCreateInfo Zodiac::Initializers::InstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& layers, std::vector<const char*>& extensions) {
	VkInstanceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appInfo;
	info.enabledLayerCount = (uint32_t)layers.size();
	info.ppEnabledLayerNames = layers.data();
	info.enabledExtensionCount = (uint32_t)extensions.size();
	info.ppEnabledExtensionNames = extensions.data();

	return info;
}

VkDeviceCreateInfo Zodiac::Initializers::DeviceCreateInfo(std::vector<VkDeviceQueueCreateInfo>& queue_create_info, VkPhysicalDeviceFeatures& physical_device_features) {
	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pQueueCreateInfos = queue_create_info.data();
	info.queueCreateInfoCount = (uint32_t)queue_create_info.size();
	info.pEnabledFeatures = &physical_device_features;

	return info;
}

VkDeviceQueueCreateInfo Zodiac::Initializers::DeviceQueueCreate(uint32_t queue_family_index, float& priority) {
	VkDeviceQueueCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = queue_family_index;
	info.queueCount = 1;
	info.pQueuePriorities = &priority;

	return info;
}

VkCommandPoolCreateInfo Zodiac::Initializers::CommandPoolCreateinfo(uint32_t queue_family_index, VkCommandPoolCreateFlags flags) {
	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.queueFamilyIndex = queue_family_index;
	info.flags = flags;

	return info;
}

VkCommandBufferAllocateInfo Zodiac::Initializers::CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count) {
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	return info;
}

VkBufferCreateInfo Zodiac::Initializers::BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage_flags) {
	VkBufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.size = size;
	info.usage = usage_flags;

	return info;
}

VkMemoryAllocateInfo Zodiac::Initializers::MemoryAllocateInfo(VkDeviceSize size, uint32_t memory_type_index) {
	VkMemoryAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = size;
	info.memoryTypeIndex = memory_type_index;

	return info;
}
