#pragma once
#include <vulkan\vulkan.h>

#include "Validation.h"
#include "VulkanConfiguration.h"
#include "SurfaceDetails.h"

namespace Zodiac { namespace Initializers {
		//Infos
		VkApplicationInfo ApplicationInfo(VulkanConfiguration& config);
		VkInstanceCreateInfo InstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& layers, std::vector<const char*>& extensions);
		VkDeviceCreateInfo DeviceCreateInfo(std::vector<VkDeviceQueueCreateInfo>& queue_create_info, VkPhysicalDeviceFeatures& physical_device_features, std::vector<const char*>& device_extensions);
		VkDeviceQueueCreateInfo DeviceQueueCreate(uint32_t queue_family_index, float& priority);
		VkSwapchainCreateInfoKHR SwapchainCreateInfo(VkSwapchainKHR& swapchain, VkSurfaceKHR& surface, VkPresentModeKHR& presentMode, SurfaceDetails* details, VkImageUsageFlags usageFlags, VkSurfaceFormatKHR surfaceFormat, VkExtent2D extent);
		VkImageViewCreateInfo ImageViewCreateInfo(VkFormat colorFormat, VkImage image, VkImageSubresourceRange subResourceRange, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
		VkImageCreateInfo ImageCreateInfo(VkFormat format, VkExtent3D extent, VkImageUsageFlags usageFlags, VkImageType imageType = VK_IMAGE_TYPE_2D);
		VkRenderPassCreateInfo RenderPassCreateInfo(std::vector<VkAttachmentDescription>& attachDesc, std::vector<VkSubpassDescription>& subpassDesc, std::vector<VkSubpassDependency>& dependencies);
		VkPipelineCacheCreateInfo PipelineCacheCreateInfo();
		VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
		VkFramebufferCreateInfo FramebufferCreateInfo(VkRenderPass renderPass, std::vector<VkImageView>& attachments, uint32_t width, uint32_t height);
		VkSemaphoreCreateInfo SemaphoreCreateInfo();
		VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags = 0);
		VkSubmitInfo SubmitInfo(VkSemaphore& present_semaphore, VkSemaphore& render_semaphore, VkPipelineStageFlags flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		VkCommandPoolCreateInfo CommandPoolCreateinfo(uint32_t queue_family_index, VkCommandPoolCreateFlags flags = 0);
		VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count);
		VkBufferCreateInfo BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage_flags);
		VkMemoryAllocateInfo MemoryAllocateInfo(VkDeviceSize size, uint32_t memory_type_index);
		VkShaderModuleCreateInfo ShaderModuleCreateInfo(std::vector<char>& code);
		VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo(std::vector<VkVertexInputBindingDescription>& vertexInputBindingDesc, std::vector<VkVertexInputAttributeDescription>& vertexInputAttrDesc);
		VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology);
		VkPipelineViewportStateCreateInfo PipelineViewportStateCreateInfo(); //For now only 1. Can be extended later.
		VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo(); //For now, no options needed<
		VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo(); //For now, no options needed
		VkPipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState colorBlendAttachmentState, uint32_t attachmentCount);
		VkPipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStates);
		VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateinfo();
		VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo(std::vector<VkPipelineShaderStageCreateInfo>& shaderStageCreateInfos, VkPipelineVertexInputStateCreateInfo& vertexInputStageCreateInfo, VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo, VkPipelineViewportStateCreateInfo& viewportStateCreateInfo, VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo, VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo, VkPipelineColorBlendStateCreateInfo& colorBlendStateCreateInfo, VkPipelineDynamicStateCreateInfo& dynamicStateCreateInfo, VkPipelineLayout& pipelineLayout, VkRenderPass& renderPass, VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo);
		VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();

		VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo();

		VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		VkRenderPassBeginInfo RenderPassBeginInfo();

		//Descriptions
		VkVertexInputBindingDescription VertexInputBindingDescription(uint32_t binding, uint32_t vertexSize);
		VkVertexInputAttributeDescription VertexInputAttributeDescription(uint32_t location, uint32_t binding, uint32_t offset);
	}
}