#pragma once
#include <vulkan/vulkan.h>
#include <vector>
//#include "VulkanInstance.h"
//#include "VulkanPhysicalDevice.h"
//#include "VulkanDevice.h"
#include "VulkanSwapChainBuffer.h"

namespace Zodiac {
	class VulkanSwapchain {
	public:
		VulkanSwapchain(/*VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkDevice* device*/);
		~VulkanSwapchain();

		uint32_t GetImageCount() const;

		VkSwapchainKHR* GetSwapchain();
		VkFormat& GetColorFormat();
		VkColorSpaceKHR& GetColorSpace();
		VkSurfaceKHR& GetSurface();
		VkPresentModeKHR& GetPresentMode();

	private:
		//VulkanInstance* m_instance;
		//VulkanPhysicalDevice* m_physicalDevice;
		//VkDevice* m_device;

		VkSwapchainKHR m_swapchain;
		VkFormat m_colorFormat;
		VkColorSpaceKHR m_colorSpace;
		VkSurfaceKHR m_surface;
		VkPresentModeKHR m_presentMode;
		uint32_t m_imageCount;
		std::vector<VkImage> m_images;
		std::vector<VulkanSwapChainBuffer> m_buffers;

		uint32_t m_queueNodeIndex = UINT32_MAX;
	};
}