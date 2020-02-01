#include <Zodiacpch.h>
#include "VulkanSwapchain.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanSwapchain::VulkanSwapchain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanDevice* device)
{
	m_instance = instance->GetInstance();
	m_physicalDevice = physicalDevice->GetPhysicalDevice();
	m_device = device->GetDevice();

	//VkSwapchainCreateInfoKHR swapchainCreateInfo = Initializers::SwapchainCreateInfo(m_swapchain, m_surface, m_presentMode);

	//ErrorCheck(vkCreateSwapchainKHR(*device, &swapchainCreateInfo, nullptr, &m_swapchain));
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

VkPresentModeKHR& Zodiac::VulkanSwapchain::GetPresentMode()
{
	return m_presentMode;
}
