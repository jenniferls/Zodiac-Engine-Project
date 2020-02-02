#pragma once
#include <vulkan/vulkan.h>
#include "VulkanSwapChainBuffer.h"
#include "VulkanDevice.h"
#include "SurfaceDetails.h"
#include "Settings.h"

namespace Zodiac {
	class VulkanSwapchain {
	public:
		VulkanSwapchain(VulkanDevice* device, SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings);
		~VulkanSwapchain();

		uint32_t GetImageCount() const;

		VkSwapchainKHR* GetSwapchain();
		VkPresentModeKHR& GetPresentMode();

	private:
		void ChooseSurfaceFormat(SurfaceDetails& details);
		void ChoosePresentMode(SurfaceDetails& details, Settings& settings);
		void ChooseExtent(SurfaceDetails& details);
		void ChooseUsageFlags(SurfaceDetails& details);

		VkDevice* m_device;

		VkSwapchainKHR m_swapchain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D m_extent2D;
		VkImageUsageFlags m_usageFlags; //The desired usage

		uint32_t m_imageCount;
		std::vector<VkImage> m_images;
		std::vector<VulkanSwapChainBuffer> m_buffers;

		uint32_t m_queueNodeIndex = UINT32_MAX;
	};
}