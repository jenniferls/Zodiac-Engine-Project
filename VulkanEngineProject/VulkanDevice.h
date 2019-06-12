#pragma once
#include <vector>

#include "Initializers.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

class VulkanDevice {
public:
	VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device);
	~VulkanDevice();

	VkDevice* GetDevice();
	VkQueue* GetComputeQueue();

private:
	VulkanInstance* m_instance;
	VulkanPhysicalDevice* m_physical_device;
	VkDevice m_device;
	VkQueue m_compute_queue;
	float m_priority;
};