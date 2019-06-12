#include "VulkanPhysicalDevice.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance* instance, VkPhysicalDevice device, QueueFamilyIndices indices) {
	m_instance = instance;
	m_device = device;
	m_family_indices = indices;

	vkGetPhysicalDeviceProperties(device, &m_device_properties);
	vkGetPhysicalDeviceFeatures(device, &m_device_features);
	vkGetPhysicalDeviceMemoryProperties(device, &m_device_memory_properties);
}

std::vector<VkPhysicalDevice> VulkanPhysicalDevice::GetAvailablePhysicalDevices(VulkanInstance* instance) {
	uint32_t device_count = 0;

	vkEnumeratePhysicalDevices(instance->GetInstance(), &device_count, nullptr);
	std::vector<VkPhysicalDevice> devices(device_count);
	vkEnumeratePhysicalDevices(instance->GetInstance(), &device_count, devices.data());

	return devices;
}

bool VulkanPhysicalDevice::PhysicalDeviceSupported(VkPhysicalDevice& device, QueueFamilyIndices& family_indices) {
	bool supported = QueueFamilySupported(device, family_indices);

	return supported;
}

bool VulkanPhysicalDevice::QueueFamilySupported(VkPhysicalDevice& device, QueueFamilyIndices& family_indices) {
	uint32_t queue_family_count = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	for (int i = 0; i < queue_families.size(); i++) {
		if (queue_families[i].queueCount > 0) {
			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				family_indices.graphics_indices = i;
			}
			if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				family_indices.compute_indices = i;
			}
		}
		if (family_indices.graphics_indices < UINT32_MAX && family_indices.compute_indices < UINT32_MAX) {
			return true;
		}
	}

	return false;
}

VulkanPhysicalDevice::~VulkanPhysicalDevice() {

}

VkPhysicalDevice& VulkanPhysicalDevice::GetPhysicalDevice() {
	return m_device;
}

QueueFamilyIndices& VulkanPhysicalDevice::GetFamilyIndices() {
	return m_family_indices;
}

VkPhysicalDeviceProperties& VulkanPhysicalDevice::GetDevicePropteries() {
	return m_device_properties;
}

VkPhysicalDeviceFeatures& VulkanPhysicalDevice::GetDeviceFeatures() {
	return m_device_features;
}

VkPhysicalDeviceMemoryProperties& VulkanPhysicalDevice::GetDeviceMemoryProperties() {
	return m_device_memory_properties;
}

VulkanPhysicalDevice* VulkanPhysicalDevice::GetPhysicalDevice(VulkanInstance* instance) {
	std::vector <VkPhysicalDevice> devices = GetAvailablePhysicalDevices(instance);
	VkPhysicalDevice secondary_device = VK_NULL_HANDLE;
	QueueFamilyIndices secondary_queue;

	for (int i = 0; i < devices.size(); i++) {
		QueueFamilyIndices queue_family;
		if (PhysicalDeviceSupported(devices[i], queue_family)) {
			VkPhysicalDeviceProperties physical_device_properties;
			vkGetPhysicalDeviceProperties(devices[i], &physical_device_properties);

			if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				return new VulkanPhysicalDevice(instance, devices[i], queue_family);
			}
			else { //GPU is not supported
				secondary_device = devices[i];
				secondary_queue = queue_family;
			}
		}
	}

	if (secondary_device == VK_NULL_HANDLE) {
		return nullptr;
	}

	return new VulkanPhysicalDevice(instance, secondary_device, secondary_queue);
}
