#include "VulkanDevice.h"

VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device) {
	m_instance = instance;
	m_physical_device = physical_device;
	m_priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queue_create_info;
	queue_create_info.push_back(Initializers::DeviceQueueCreate(m_physical_device->GetFamilyIndices().compute_indices, m_priority));

	VkDeviceCreateInfo device_info = Initializers::DeviceCreateInfo(queue_create_info, m_physical_device->GetDeviceFeatures());

	vkCreateDevice(m_physical_device->GetPhysicalDevice(), &device_info, nullptr, &m_device);
	vkGetDeviceQueue(m_device, m_physical_device->GetFamilyIndices().compute_indices, 0, &m_compute_queue);
}

VulkanDevice::~VulkanDevice() {
	vkDestroyDevice(m_device, nullptr);
}

VkDevice* VulkanDevice::GetDevice() {
	return &m_device;
}

VkQueue* VulkanDevice::GetComputeQueue() {
	return &m_compute_queue;
}
