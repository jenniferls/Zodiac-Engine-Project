#include "VulkanSwapchain.h"

Zodiac::VulkanSwapchain::VulkanSwapchain(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice* device)
{
	m_instance = instance;
	m_physicalDevice = physicalDevice;
	m_device = device;

}

Zodiac::VulkanSwapchain::~VulkanSwapchain()
{

}

uint32_t Zodiac::VulkanSwapchain::GetImageCount() const {
	return m_imageCount;
}

VkSwapchainKHR* Zodiac::VulkanSwapchain::GetSwapchain()
{
	return &m_swapchain;
}

VkFormat& Zodiac::VulkanSwapchain::GetColorFormat()
{
	return m_colorFormat;
}

VkColorSpaceKHR& Zodiac::VulkanSwapchain::GetColorSpace()
{
	return m_colorSpace;
}

VkSurfaceKHR& Zodiac::VulkanSwapchain::GetSurface()
{
	return m_surface;
}

VkPresentModeKHR & Zodiac::VulkanSwapchain::GetPresentMode()
{
	return m_presentMode;
}
