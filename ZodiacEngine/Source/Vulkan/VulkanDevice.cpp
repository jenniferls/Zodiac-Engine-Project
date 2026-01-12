#include "Zodiacpch.h"
#include "VulkanDevice.h"
#include "Validation.h"
#include "Initializers.h"
#include "VulkanFence.h"

Zodiac::VulkanDevice::VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device) {
	m_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME); //Device extensions
	m_extensions.push_back("VK_KHR_synchronization2");
	m_extensions.push_back("VK_KHR_create_renderpass2");
	m_extensions.push_back("VK_KHR_depth_stencil_resolve");
	m_extensions.push_back("VK_KHR_shader_draw_parameters");

	bool dynamicRenderingSupported = physical_device->IsExtensionSupported(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
	int32_t instanceVersion = instance->GetInstanceVersion();
	if (dynamicRenderingSupported) {
		if (instance->IsInstanceVersionOrAbove(1, 3)) {
			printf("The Vulkan instance and device support dynamic rendering as a core feature\n");
		}
		else{
			m_extensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		}
	}
	else {
		assert(0 && "Dynamic rendering not supported.");
	}

	if (physical_device->GetDeviceFeatures().geometryShader == VK_FALSE) {
		std::cout << "The Geometry Shader is not supported!" << std::endl;
	}
	if (physical_device->GetDeviceFeatures().tessellationShader == VK_FALSE) {
		std::cout << "The Tessellation Shader is not supported!" << std::endl;
	}

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

	VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
		.pNext = NULL,
		.dynamicRendering = VK_TRUE
	};

	VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutFeature = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
		.pNext = &dynamicRenderingFeature,
		.scalarBlockLayout = VK_TRUE
	};

	VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
		.pNext = &scalarBlockLayoutFeature
	};

	VkDeviceCreateInfo device_info = Initializers::DeviceCreateInfo(queue_create_info, m_physical_device->GetDeviceFeatures(), m_extensions, &descriptorIndexingFeatures);
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

VkCommandPool& Zodiac::VulkanDevice::GetGraphicsCommandPool() {
	return m_graphics_command_pool;
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

VkCommandBuffer Zodiac::VulkanDevice::GetCommandBuffer(bool begin) {
	VkCommandBuffer buffer;
	VkCommandBufferAllocateInfo cmdBufAllocateInfo = Initializers::CommandBufferAllocateInfo(m_graphics_command_pool, 1);
	ErrorCheck(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &buffer));

	if (begin) {
		VkCommandBufferBeginInfo cmdBufInfo = Initializers::CommandBufferBeginInfo();
		ErrorCheck(vkBeginCommandBuffer(buffer, &cmdBufInfo));
	}

	return buffer;
}

void Zodiac::VulkanDevice::CreateCommandBuffers(int count, VkCommandBuffer* buffers) {
	VkCommandBufferAllocateInfo cmdBufAllocateInfo = Initializers::CommandBufferAllocateInfo(m_graphics_command_pool, count);
	ErrorCheck(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, buffers));
	printf("%d command buffers created\n", count);
}

void Zodiac::VulkanDevice::FreeCommandBuffers(uint32_t count, const VkCommandBuffer* buffers) {
	vkQueueWaitIdle(m_graphics_queue); //Only graphics queue for now
	vkFreeCommandBuffers(m_device, m_graphics_command_pool, count, buffers);
}

void Zodiac::VulkanDevice::BeginCommandBuffer(VkCommandBuffer& cmdBuffer, VkCommandBufferUsageFlags usage) {
	VkCommandBufferBeginInfo cmdBufInfo = Initializers::CommandBufferBeginInfo(usage);
	ErrorCheck(vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo));
}

void Zodiac::VulkanDevice::FlushCommandBuffer(VkCommandBuffer& cmdBuffer, VkQueue& submitQueue, VkCommandPool& commandPool) {
	assert(cmdBuffer != VK_NULL_HANDLE);

	ErrorCheck(vkEndCommandBuffer(cmdBuffer));
	VkSubmitInfo submitInfo = Initializers::SubmitInfo(cmdBuffer);
	VkFenceCreateInfo fenceinfo = Initializers::FenceCreateInfo();
	VkFence fence;
	ErrorCheck(vkCreateFence(m_device, &fenceinfo, nullptr, &fence));
	//std::cout << (int)fence << std::endl;
	ErrorCheck(vkQueueSubmit(submitQueue, 1, &submitInfo, fence));
	ErrorCheck(vkWaitForFences(m_device, 1, &fence, VK_TRUE, 100000000000)); //TODO: Perhaps create a define for a default fence timeout

	vkDestroyFence(m_device, fence, nullptr);
	vkFreeCommandBuffers(m_device, commandPool, 1, &cmdBuffer);
}