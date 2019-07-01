#include "VulkanDevice.h"

Zodiac::VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device) {
	m_instance = instance;
	m_physical_device = physical_device;
	m_priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queue_create_info;
	queue_create_info.push_back(Initializers::DeviceQueueCreate(m_physical_device->GetFamilyIndices().compute_indices, m_priority));

	VkDeviceCreateInfo device_info = Initializers::DeviceCreateInfo(queue_create_info, m_physical_device->GetDeviceFeatures());
	VkCommandPoolCreateInfo compute_pool_info = Initializers::CommandPoolCreateinfo(m_physical_device->GetFamilyIndices().compute_indices);

	ErrorCheck(vkCreateDevice(m_physical_device->GetPhysicalDevice(), &device_info, nullptr, &m_device));
	vkGetDeviceQueue(m_device, m_physical_device->GetFamilyIndices().compute_indices, 0, &m_compute_queue);
	ErrorCheck(vkCreateCommandPool(m_device, &compute_pool_info, nullptr, &m_compute_command_pool));
}

Zodiac::VulkanDevice::~VulkanDevice() {
	vkDestroyCommandPool(m_device, m_compute_command_pool, nullptr);
	vkDestroyDevice(m_device, nullptr);
	m_compute_command_pool = nullptr;
	m_device = nullptr;
}

VkDevice* Zodiac::VulkanDevice::GetDevice() {
	return &m_device;
}

VkQueue* Zodiac::VulkanDevice::GetComputeQueue() {
	return &m_compute_queue;
}

VkCommandPool& Zodiac::VulkanDevice::GetComputeCommandPool() {
	return m_compute_command_pool;
}

Zodiac::VulkanPhysicalDevice* Zodiac::VulkanDevice::GetPhysicalDevice() {
	return m_physical_device;
}

void Zodiac::VulkanDevice::GetComputeCommand(VkCommandBuffer* buffers, uint32_t count) {
	VkCommandBufferAllocateInfo command_buffer_alloc_info = Initializers::CommandBufferAllocateInfo(m_compute_command_pool, count);
	ErrorCheck(vkAllocateCommandBuffers(m_device, &command_buffer_alloc_info, buffers));
}

void Zodiac::VulkanDevice::FreeComputeCommand(VkCommandBuffer* buffers, uint32_t count) {
	vkFreeCommandBuffers(m_device, m_compute_command_pool, count, buffers);
}
