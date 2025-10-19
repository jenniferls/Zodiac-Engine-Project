#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include "SurfaceDetails.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

namespace Zodiac {
	class VulkanSurface {
	public:
		VulkanSurface(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, void* window);
		~VulkanSurface();

		void QuerySurfaceDetails(VulkanPhysicalDevice* physicalDevice);

		SurfaceDetails& GetSurfaceDetails();
		VkSurfaceKHR& GetSurface();

	private:
		VulkanInstance* m_instance;
		VkSurfaceKHR m_surface;
		SurfaceDetails m_surfaceDetails;
	};
}