#pragma once
#include <vulkan/vulkan.h>

#include "VulkanInstance.h"
#include "QueueFamilyIndices.h"

namespace Zodiac {
	class VulkanPhysicalDevice {
	public:
		~VulkanPhysicalDevice();

		VkPhysicalDevice& GetPhysicalDevice();
		QueueFamilyIndices& GetFamilyIndices();
		VkPhysicalDeviceProperties& GetDeviceProperties();
		VkPhysicalDeviceFeatures& GetDeviceFeatures();
		VkPhysicalDeviceMemoryProperties& GetDeviceMemoryProperties();

		static VulkanPhysicalDevice* GetPhysicalDevice(VulkanInstance* instance);

	private:
		VulkanPhysicalDevice(VulkanInstance* instance, VkPhysicalDevice device, QueueFamilyIndices indices); //Private constructor only called for getting a new physical device
		static std::vector<VkPhysicalDevice> GetAvailablePhysicalDevices(VulkanInstance* instance);
		static bool PhysicalDeviceSupported(VkPhysicalDevice& device, QueueFamilyIndices& family_indices);
		static bool QueueFamilySupported(VkPhysicalDevice& device, QueueFamilyIndices& family_indices);

		VulkanInstance* m_instance;
		VkPhysicalDevice m_device;
		QueueFamilyIndices m_family_indices;
		VkPhysicalDeviceProperties m_device_properties;
		VkPhysicalDeviceFeatures m_device_features;
		VkPhysicalDeviceMemoryProperties m_device_memory_properties;
	};
}