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

VkDeviceCreateInfo Zodiac::Initializers::DeviceCreateInfo(std::vector<VkDeviceQueueCreateInfo>& queue_create_info, VkPhysicalDeviceFeatures& physical_device_features, std::vector<const char*>& device_extensions) {
	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pQueueCreateInfos = queue_create_info.data();
	info.queueCreateInfoCount = (uint32_t)queue_create_info.size();
	info.pEnabledFeatures = &physical_device_features;
	info.ppEnabledExtensionNames = device_extensions.data();
	info.enabledExtensionCount = (uint32_t)device_extensions.size();

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

VkSwapchainCreateInfoKHR Zodiac::Initializers::SwapchainCreateInfo(VkSwapchainKHR& swapchain, VkSurfaceKHR& surface, VkPresentModeKHR& presentMode)
{
	VkSwapchainCreateInfoKHR info = {};

	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.presentMode = presentMode;
	info.surface = surface;
	info.oldSwapchain = swapchain;

	return info;
}

VkSemaphoreCreateInfo Zodiac::Initializers::SemaphoreCreateInfo()
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	return info;
}

VkSubmitInfo Zodiac::Initializers::SubmitInfo(VkSemaphore& present_semaphore, VkSemaphore& render_semaphore, VkPipelineStageFlags flags)
{
	VkSubmitInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.pWaitDstStageMask = &flags;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &present_semaphore;
	info.signalSemaphoreCount = 1;
	info.pSignalSemaphores = &render_semaphore;

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

VkDebugUtilsMessengerCreateInfoEXT Zodiac::Initializers::DebugUtilsMessengerCreateInfo() {
	VkDebugUtilsMessengerCreateInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = Zodiac::DebugCallback;
	info.pUserData = nullptr;

	return info;
}
