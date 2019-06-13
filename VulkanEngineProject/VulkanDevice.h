#pragma once
#include <vector>
#include <vulkan\vulkan.h>

#include "Validation.h"
#include "Initializers.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

class VulkanDevice {
public:
	VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device);
	~VulkanDevice();

	VkDevice* GetDevice();
	VkQueue* GetComputeQueue();
	VkCommandPool& GetComputeCommandPool();
	VulkanPhysicalDevice* GetPhysicalDevice();

	void GetComputeCommand(VkCommandBuffer* buffers, uint32_t count);
	void FreeComputeCommand(VkCommandBuffer* buffers, uint32_t count);

private:
	VulkanInstance* m_instance;
	VulkanPhysicalDevice* m_physical_device;
	VkDevice m_device;
	VkQueue m_compute_queue;
	float m_priority;
	VkCommandPool m_compute_command_pool;
};