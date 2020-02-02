#include <Zodiacpch.h>
#include "VulkanSurface.h"
#include "Validation.h"

Zodiac::VulkanSurface::VulkanSurface(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, void* window) {
	m_instance = instance;
	m_surface = VK_NULL_HANDLE;
	ErrorCheck(glfwCreateWindowSurface(instance->GetInstance(), (GLFWwindow*)window, NULL, &m_surface));
	QuerySurfaceDetails(physicalDevice);
}

Zodiac::VulkanSurface::~VulkanSurface() {
	vkDestroySurfaceKHR(m_instance->GetInstance(), m_surface, NULL);
}

Zodiac::SurfaceDetails& Zodiac::VulkanSurface::GetSurfaceDetails() {
	return m_surfaceDetails;
}

VkSurfaceKHR& Zodiac::VulkanSurface::GetSurface() {
	return m_surface;
}

void Zodiac::VulkanSurface::QuerySurfaceDetails(VulkanPhysicalDevice* physicalDevice) {
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &m_surfaceDetails.capabilities);

	//Lazy implementation, but will work in many cases.
	//We test if the graphics index we got also suppports presentation
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice->GetPhysicalDevice(), physicalDevice->GetFamilyIndices().graphics_indices, m_surface, &presentSupport);
	if (presentSupport) {
		physicalDevice->GetFamilyIndices().present_indices = physicalDevice->GetFamilyIndices().graphics_indices;
	}
	else {
		//There should be some actual error handling here
		std::cout << "Warning: Present not supported!" << std::endl;
	}

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->GetPhysicalDevice(), m_surface, &formatCount, m_surfaceDetails.supported_formats.data());
	if (formatCount != 0) {
		m_surfaceDetails.supported_formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->GetPhysicalDevice(), m_surface, &formatCount, m_surfaceDetails.supported_formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		m_surfaceDetails.supported_present_modes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &presentModeCount, m_surfaceDetails.supported_present_modes.data());
	}
}
