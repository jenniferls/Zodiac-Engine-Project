#include <Zodiacpch.h>
#include "VulkanSwapchain.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings) {
	m_device = device->GetDevice();
	m_swapchain = VK_NULL_HANDLE;

	ChooseSurfaceFormat(details);
	ChooseDepthFormat(device->GetPhysicalDevice()->GetPhysicalDevice()); //TODO: Don't do this for every swapchain...
	ChoosePresentMode(details, settings);
	ChooseExtent(details);
	ChooseUsageFlags(details);
	assert(m_usageFlags != -1);

	CreateSwapchain(details, surface, settings);
	CreateImageViews();
	CreateDepthResources(device);

	std::cout << "Swapchain initialized." << std::endl;
}

Zodiac::VulkanSwapchain::~VulkanSwapchain() {
	Cleanup();
}

uint32_t Zodiac::VulkanSwapchain::GetImageCount() const {
	return m_imageCount;
}

void Zodiac::VulkanSwapchain::Recreate(SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings, VulkanDevice* device)
{
	Cleanup();

	ChooseSurfaceFormat(details);
	ChoosePresentMode(details, settings);
	ChooseExtent(details);
	ChooseUsageFlags(details);
	assert(m_usageFlags != -1);

	CreateSwapchain(details, surface, settings);
	CreateImageViews();
	CreateDepthResources(device);
}

VkSwapchainKHR* Zodiac::VulkanSwapchain::GetSwapchain() {
	return &m_swapchain;
}

VkPresentModeKHR& Zodiac::VulkanSwapchain::GetPresentMode() {
	return m_presentMode;
}

VkSurfaceFormatKHR& Zodiac::VulkanSwapchain::GetSurfaceFormat() {
	return m_surfaceFormat;
}

VkFormat& Zodiac::VulkanSwapchain::GetDepthFormat() {
	return m_depthFormat;
}

std::vector<Zodiac::VulkanSwapChainBuffer>& Zodiac::VulkanSwapchain::GetBuffers() {
	return m_buffers;
}

Zodiac::VulkanSwapChainBuffer& Zodiac::VulkanSwapchain::GetBuffer(uint32_t index) {
	return m_buffers[index];
}

Zodiac::DepthStencil& Zodiac::VulkanSwapchain::GetDepthStencil() {
	return m_depthStencil;
}

VkExtent2D Zodiac::VulkanSwapchain::GetExtent2D() const {
	return m_extent2D;
}

const VkImage& Zodiac::VulkanSwapchain::GetImage(int index) const {
	if (index >= m_images.size()) {
		printf("Invalid image index %d\n", index);
		exit(1);
	}

	return m_images[index];
}

bool Zodiac::VulkanSwapchain::SurfaceHasStencilComponent(VkFormat format)
{
	return ((format == VK_FORMAT_D32_SFLOAT_S8_UINT) || (format == VK_FORMAT_D24_UNORM_S8_UINT));
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

void Zodiac::VulkanSwapchain::ChooseDepthFormat(VkPhysicalDevice& physicalDevice) {
	std::vector<VkFormat> depthFormats = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	for (auto& format : depthFormats) {
		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
		// Format must support depth stencil attachment for optimal tiling
		if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			m_depthFormat = format;
			break;
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

	//VkPresentModeKHR bestPresentMode = VK_PRESENT_MODE_FIFO_KHR; //Should be always available
	//if (!settings.vsync) {
	//	for (const auto& presentMode : details.supported_present_modes) {
	//		if (presentMode == /*VK_PRESENT_MODE_IMMEDIATE_KHR*/VK_PRESENT_MODE_MAILBOX_KHR) {
	//			bestPresentMode = presentMode;
	//			break;
	//		}
	//		if (presentMode == /*VK_PRESENT_MODE_MAILBOX_KHR*/VK_PRESENT_MODE_IMMEDIATE_KHR) {
	//			bestPresentMode = presentMode; //Fallback
	//		}
	//	}
	//}

	//m_presentMode = bestPresentMode;
}

void Zodiac::VulkanSwapchain::ChooseExtent(SurfaceDetails& details) {
	if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		//Get from window size
		m_extent2D = details.capabilities.currentExtent;
	}
	else {
		VkExtent2D swap_chain_extent = { 640, 480 }; //Default

		swap_chain_extent.width = glm::clamp(swap_chain_extent.width, details.capabilities.minImageExtent.width, details.capabilities.maxImageExtent.width);
		swap_chain_extent.height = glm::clamp(swap_chain_extent.height, details.capabilities.minImageExtent.height, details.capabilities.maxImageExtent.height);

		m_extent2D = swap_chain_extent;
	}
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

void Zodiac::VulkanSwapchain::CreateSwapchain(SurfaceDetails& details, VkSurfaceKHR& surface, Settings& settings)
{
	VkSwapchainCreateInfoKHR swapchainCreateInfo = Initializers::SwapchainCreateInfo(m_swapchain, surface, m_presentMode, &details, m_usageFlags, m_surfaceFormat, m_extent2D);

	ErrorCheck(vkCreateSwapchainKHR(*m_device, &swapchainCreateInfo, nullptr, &m_swapchain));
	ErrorCheck(vkGetSwapchainImagesKHR(*m_device, m_swapchain, &m_imageCount, NULL)); //Obtain number of images
	m_images.resize(m_imageCount);
	ErrorCheck(vkGetSwapchainImagesKHR(*m_device, m_swapchain, &m_imageCount, m_images.data()));
	m_buffers.resize(m_imageCount);
}

void Zodiac::VulkanSwapchain::CreateImageViews()
{
	VkImageSubresourceRange subResourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	for (uint32_t i = 0; i < m_imageCount; i++) {
		m_buffers[i].image = m_images[i];
		VkImageViewCreateInfo colorAttachmentView = Initializers::ImageViewCreateInfo(m_surfaceFormat.format, m_images[i], subResourceRange);
		ErrorCheck(vkCreateImageView(*m_device, &colorAttachmentView, nullptr, &m_buffers[i].view));
	}
}

void Zodiac::VulkanSwapchain::CreateDepthResources(VulkanDevice* device)
{
	//Depth attachment
	VkImageCreateInfo image = Initializers::ImageCreateInfo(m_depthFormat, VkExtent3D({ m_extent2D.width, m_extent2D.height, 1 }), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
	ErrorCheck(vkCreateImage(*m_device, &image, nullptr, &m_depthStencil.image));
	VkMemoryRequirements mem_reqs;
	vkGetImageMemoryRequirements(*m_device, m_depthStencil.image, &mem_reqs);
	VkMemoryAllocateInfo allocateInfo = Initializers::MemoryAllocateInfo(mem_reqs.size, GetMemoryTypeIndex(device->GetPhysicalDevice()->GetDeviceMemoryProperties(), mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	ErrorCheck(vkAllocateMemory(*m_device, &allocateInfo, nullptr, &m_depthStencil.mem));
	ErrorCheck(vkBindImageMemory(*m_device, m_depthStencil.image, m_depthStencil.mem, 0));
	//Image view for depth attachment
	VkImageSubresourceRange subResourceRangeDepth = { VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 };
	VkImageViewCreateInfo depthStencilView = Initializers::ImageViewCreateInfo(m_depthFormat, m_depthStencil.image, subResourceRangeDepth);
	// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
	if (depthStencilView.format >= VK_FORMAT_D16_UNORM_S8_UINT) {
		depthStencilView.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	ErrorCheck(vkCreateImageView(*m_device, &depthStencilView, nullptr, &m_depthStencil.view));
}

void Zodiac::VulkanSwapchain::Cleanup()
{
	vkFreeMemory(*m_device, m_depthStencil.mem, nullptr);
	vkDestroyImage(*m_device, m_depthStencil.image, nullptr);
	vkDestroyImageView(*m_device, m_depthStencil.view, nullptr);

	for (uint32_t i = 0; i < m_imageCount; i++) {
		vkDestroyImageView(*m_device, m_buffers[i].view, nullptr);
	};
	vkDestroySwapchainKHR(*m_device, m_swapchain, nullptr);
	m_swapchain = VK_NULL_HANDLE;
}

uint32_t Zodiac::VulkanSwapchain::GetMemoryTypeIndex(VkPhysicalDeviceMemoryProperties physDeviceMemProps, uint32_t memTypeBits, VkMemoryPropertyFlagBits flags) {
	for (uint32_t i = 0; i < physDeviceMemProps.memoryTypeCount; i++) {
		if ((memTypeBits & 1) == 1) {
			if ((physDeviceMemProps.memoryTypes[i].propertyFlags & flags) == flags) {
				return i;
			}
		}
		memTypeBits >>= 1;
	}

	throw "Could not find a suitable memory type!";
}
