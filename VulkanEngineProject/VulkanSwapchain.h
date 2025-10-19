#pragma once
#include <vulkan/vulkan.h>
#include "VulkanSwapChainBuffer.h"
#include "VulkanDevice.h"
#include "SurfaceDetails.h"
#include "Settings.h"
#include "DepthStencil.h"

namespace Zodiac {
#define PRINT_PRESENT_MODE(mode) \
    std::cout << "Selected present mode: " << ( \
        (mode) == VK_PRESENT_MODE_IMMEDIATE_KHR ? "IMMEDIATE" : \
        (mode) == VK_PRESENT_MODE_MAILBOX_KHR   ? "MAILBOX"   : \
        (mode) == VK_PRESENT_MODE_FIFO_KHR      ? "FIFO"      : \
        (mode) == VK_PRESENT_MODE_FIFO_RELAXED_KHR ? "FIFO_RELAXED" : \
        "UNKNOWN") << std::endl;

	class VulkanSwapchain {
	public:
		VulkanSwapchain(VulkanDevice* device, SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings);
		~VulkanSwapchain();

		uint32_t GetImageCount() const;

		void Recreate(SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings, VulkanDevice* device);

		VkSwapchainKHR* GetSwapchain();
		VkPresentModeKHR& GetPresentMode();
		VkSurfaceFormatKHR& GetSurfaceFormat();
		VkFormat& GetDepthFormat();
		std::vector<VulkanSwapChainBuffer>& GetBuffers();
		VulkanSwapChainBuffer& GetBuffer(uint32_t index);
		DepthStencil& GetDepthStencil();
		VkExtent2D GetExtent2D() const;
		const VkImage& GetImage(int index) const;
		bool SurfaceHasStencilComponent(VkFormat format);

	private:
		void ChooseSurfaceFormat(SurfaceDetails& details);
		void ChooseDepthFormat(VkPhysicalDevice& physicalDevice);
		void ChoosePresentMode(SurfaceDetails& details, Settings& settings);
		void ChooseExtent(SurfaceDetails& details);
		void ChooseUsageFlags(SurfaceDetails& details);

		void CreateSwapchain(SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings);
		void CreateImageViews();
		void CreateDepthResources(VulkanDevice* device);

		void Cleanup();

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