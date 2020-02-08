#include <Zodiacpch.h>
#include "VulkanSwapchain.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings) {
	m_device = device->GetDevice();
	m_swapchain = VK_NULL_HANDLE;

	ChooseSurfaceFormat(details);
	ChoosePresentMode(details, settings);
	ChooseExtent(details);
	ChooseUsageFlags(details);
	assert(m_usageFlags != -1);

	VkSwapchainCreateInfoKHR swapchainCreateInfo = Initializers::SwapchainCreateInfo(m_swapchain, surface, m_presentMode, &details, m_usageFlags, m_surfaceFormat, m_extent2D);

	ErrorCheck(vkCreateSwapchainKHR(*m_device, &swapchainCreateInfo, nullptr, &m_swapchain));
	ErrorCheck(vkGetSwapchainImagesKHR(*m_device, m_swapchain, &m_imageCount, NULL));
	m_images.resize(m_imageCount);
	ErrorCheck(vkGetSwapchainImagesKHR(*m_device, m_swapchain, &m_imageCount, m_images.data()));
	m_buffers.resize(m_imageCount);
	for (uint32_t i = 0; i < m_imageCount; i++) {
		m_buffers[i].image = m_images[i];
		VkImageViewCreateInfo colorAttachmentView = Initializers::ImageViewCreateInfo(m_surfaceFormat.format, m_images[i]);
		ErrorCheck(vkCreateImageView(*m_device, &colorAttachmentView, nullptr, &m_buffers[i].view));
	}
}

Zodiac::VulkanSwapchain::~VulkanSwapchain() {
	for (uint32_t i = 0; i < m_imageCount; i++) {
		vkDestroyImageView(*m_device, m_buffers[i].view, nullptr);
	};
	vkDestroySwapchainKHR(*m_device, m_swapchain, nullptr);
}

uint32_t Zodiac::VulkanSwapchain::GetImageCount() const {
	return m_imageCount;
}

VkSwapchainKHR* Zodiac::VulkanSwapchain::GetSwapchain() {
	return &m_swapchain;
}

VkPresentModeKHR& Zodiac::VulkanSwapchain::GetPresentMode() {
	return m_presentMode;
}

void Zodiac::VulkanSwapchain::ChooseSurfaceFormat(SurfaceDetails& details) {
	if ((details.supported_formats.size() == 1) && (details.supported_formats[0].format == VK_FORMAT_UNDEFINED)) {
		m_surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		m_surfaceFormat.colorSpace = details.supported_formats[0].colorSpace;
	}
	else {
		bool found_B8G8R8A8_UNORM = false;
		for (auto&& surfaceFormat : details.supported_formats) {
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
				m_surfaceFormat.format = surfaceFormat.format;
				m_surfaceFormat.colorSpace = surfaceFormat.colorSpace;
				found_B8G8R8A8_UNORM = true;
				break;
			}
		}

		// in case VK_FORMAT_B8G8R8A8_UNORM is not available, select the first available color format
		if (!found_B8G8R8A8_UNORM) {
			m_surfaceFormat.format = details.supported_formats[0].format;
			m_surfaceFormat.colorSpace = details.supported_formats[0].colorSpace;
		}
	}
}

void Zodiac::VulkanSwapchain::ChoosePresentMode(SurfaceDetails& details, Settings& settings) {
	if (settings.vsync) {
		m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
	}
	else {
		bool foundPresentMode = false;

		for (const auto& presentMode : details.supported_present_modes) {
			if (presentMode == (VK_PRESENT_MODE_IMMEDIATE_KHR || VK_PRESENT_MODE_MAILBOX_KHR)) {
				m_presentMode = presentMode;
				foundPresentMode = true;
				break;
			}
		}

		if (!foundPresentMode) {
			m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
		}
	}
}

void Zodiac::VulkanSwapchain::ChooseExtent(SurfaceDetails& details) {
	if (details.capabilities.currentExtent.width == -1) {
		VkExtent2D swap_chain_extent = { 640, 480 };
		if (swap_chain_extent.width < details.capabilities.minImageExtent.width) {
			swap_chain_extent.width = details.capabilities.minImageExtent.width;
		}
		if (swap_chain_extent.height < details.capabilities.minImageExtent.height) {
			swap_chain_extent.height = details.capabilities.minImageExtent.height;
		}
		if (swap_chain_extent.width > details.capabilities.maxImageExtent.width) {
			swap_chain_extent.width = details.capabilities.maxImageExtent.width;
		}
		if (swap_chain_extent.height > details.capabilities.maxImageExtent.height) {
			swap_chain_extent.height = details.capabilities.maxImageExtent.height;
		}
		m_extent2D = swap_chain_extent;
	}

	// Most of the cases we define size of the swap_chain images equal to current window's size
	m_extent2D = details.capabilities.currentExtent;
}

void Zodiac::VulkanSwapchain::ChooseUsageFlags(SurfaceDetails& details) {
	if (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
		m_usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}
	else {
		std::cout << "VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain!" << std::endl
			<< "Supported swap chain's image usages include:" << std::endl
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ? "    VK_IMAGE_USAGE_TRANSFER_SRC\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ? "    VK_IMAGE_USAGE_TRANSFER_DST\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT ? "    VK_IMAGE_USAGE_SAMPLED\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT ? "    VK_IMAGE_USAGE_STORAGE\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "")
			<< (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "")
			<< std::endl;
		m_usageFlags = static_cast<VkImageUsageFlags>(-1);
	}
}
