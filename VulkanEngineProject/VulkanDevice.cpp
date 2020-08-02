#include "Zodiacpch.h"
#include "VulkanDevice.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device) {
	m_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME); //Device extension

	m_instance = instance;
	m_physical_device = physical_device;
	m_priority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queue_create_info;
	queue_create_info.push_back(Initializers::DeviceQueueCreate(m_physical_device->GetFamilyIndices().compute_indices, m_priority));
	if (m_physical_device->GetFamilyIndices().graphics_indices != m_physical_device->GetFamilyIndices().compute_indices) {
		queue_create_info.push_back(Initializers::DeviceQueueCreate(m_physical_device->GetFamilyIndices().graphics_indices, m_priority));
	}
	else { //If they're the same we use the same queue
		m_physical_device->GetFamilyIndices().graphics_indices = m_physical_device->GetFamilyIndices().compute_indices;
	}

	VkDeviceCreateInfo device_info = Initializers::DeviceCreateInfo(queue_create_info, m_physical_device->GetDeviceFeatures(), m_extensions);
	VkCommandPoolCreateInfo compute_pool_info = Initializers::CommandPoolCreateinfo(m_physical_device->GetFamilyIndices().compute_indices);
	VkCommandPoolCreateInfo graphics_pool_info = Initializers::CommandPoolCreateinfo(m_physical_device->GetFamilyIndices().graphics_indices, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	ErrorCheck(vkCreateDevice(m_physical_device->GetPhysicalDevice(), &device_info, nullptr, &m_device));

	vkGetDeviceQueue(m_device, m_physical_device->GetFamilyIndices().compute_indices, 0, &m_compute_queue);
	vkGetDeviceQueue(m_device, m_physical_device->GetFamilyIndices().graphics_indices, 0, &m_graphics_queue);

	ErrorCheck(vkCreateCommandPool(m_device, &compute_pool_info, nullptr, &m_compute_command_pool));
	ErrorCheck(vkCreateCommandPool(m_device, &graphics_pool_info, nullptr, &m_graphics_command_pool));
}

Zodiac::VulkanDevice::~VulkanDevice() {
	vkDestroyCommandPool(m_device, m_compute_command_pool, nullptr);
	vkDestroyCommandPool(m_device, m_graphics_command_pool, nullptr);
	vkDestroyDevice(m_device, nullptr);
	m_compute_command_pool = nullptr;
	m_graphics_command_pool = nullptr;
	m_device = nullptr;
}

VkDevice* Zodiac::VulkanDevice::GetDevice() {
	return &m_device;
}

VkQueue* Zodiac::VulkanDevice::GetComputeQueue() {
	return &m_compute_queue;
}

VkQueue* Zodiac::VulkanDevice::GetGraphicsQueue() {
	return &m_graphics_queue;
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

void Zodiac::VulkanDevice::GetGraphicsCommand(VkCommandBuffer* buffers, uint32_t count) {
	VkCommandBufferAllocateInfo graphics_buffer_alloc_info = Initializers::CommandBufferAllocateInfo(m_graphics_command_pool, count);
	ErrorCheck(vkAllocateCommandBuffers(m_device, &graphics_buffer_alloc_info, buffers));
}

void Zodiac::VulkanDevice::SubmitGraphicsCommand(VkCommandBuffer* buffers) {
	VkCommandBufferBeginInfo cmdBufferBeginInfo = Initializers::CommandBufferBeginInfo();
	ErrorCheck(vkBeginCommandBuffer(*buffers, &cmdBufferBeginInfo));
}

void Zodiac::VulkanDevice::FreeComputeCommand(VkCommandBuffer* buffers, uint32_t count) {
	vkFreeCommandBuffers(m_device, m_compute_command_pool, count, buffers);
}

void Zodiac::VulkanDevice::FreeGraphicsCommand(VkCommandBuffer* buffers, uint32_t count) {
	vkFreeCommandBuffers(m_device, m_graphics_command_pool, count, buffers);
}
