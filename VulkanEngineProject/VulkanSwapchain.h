#pragma once
#include <vulkan/vulkan.h>
#include "VulkanSwapChainBuffer.h"
#include "VulkanDevice.h"
#include "SurfaceDetails.h"
#include "Settings.h"
#include "DepthStencil.h"

namespace Zodiac {
	class VulkanSwapchain {
	public:
		VulkanSwapchain(VulkanDevice* device, SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings);
		~VulkanSwapchain();

		uint32_t GetImageCount() const;

		VkSwapchainKHR* GetSwapchain();
		VkPresentModeKHR& GetPresentMode();
		VkSurfaceFormatKHR& GetSurfaceFormat();
		VkFormat& GetDepthFormat();
		std::vector<VulkanSwapChainBuffer>& GetBuffers();
		VulkanSwapChainBuffer& GetBuffer(uint32_t index);
		DepthStencil& GetDepthStencil();
		VkExtent2D GetExtent2D() const;

	private:
		void ChooseSurfaceFormat(SurfaceDetails& details);
		void ChooseDepthFormat(VkPhysicalDevice& physicalDevice);
		void ChoosePresentMode(SurfaceDetails& details, Settings& settings);
		void ChooseExtent(SurfaceDetails& details);
		void ChooseUsageFlags(SurfaceDetails& details);

		uint32_t GetMemoryTypeIndex(VkPhysicalDeviceMemoryProperties physDeviceMemProps, uint32_t memTypeBits, VkMemoryPropertyFlagBits flags);

		VkDevice* m_device;

		VkSwapchainKHR m_swapchain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D m_extent2D;
		VkImageUsageFlags m_usageFlags; //The desired usage

		uint32_t m_imageCount;
		std::vector<VkImage> m_images;
		std::vector<VulkanSwapChainBuffer> m_buffers;

		DepthStencil m_depthStencil;
		VkFormat m_depthFormat;

		uint32_t m_queueNodeIndex = UINT32_MAX;
	};
}