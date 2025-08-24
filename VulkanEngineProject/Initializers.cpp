#include <Zodiacpch.h>
#include "Initializers.h"

VkApplicationInfo Zodiac::Initializers::ApplicationInfo(VulkanConfiguration& config) {
	VkApplicationInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	info.pApplicationName = config.app_name;
	info.applicationVersion = config.app_version;
	info.pEngineName = config.engine_name;
	info.engineVersion = config.engine_version;
	info.apiVersion = config.api_version;
	return info;
}

VkInstanceCreateInfo Zodiac::Initializers::InstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& layers, std::vector<const char*>& extensions) {
	VkInstanceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appInfo;
	info.enabledLayerCount = (uint32_t)layers.size();
	info.ppEnabledLayerNames = layers.data();
	info.enabledExtensionCount = (uint32_t)extensions.size();
	info.ppEnabledExtensionNames = extensions.data();

	return info;
}

VkDeviceCreateInfo Zodiac::Initializers::DeviceCreateInfo(std::vector<VkDeviceQueueCreateInfo>& queue_create_info, VkPhysicalDeviceFeatures& physical_device_features, std::vector<const char*>& device_extensions, const void* pNext) {
	VkDeviceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pNext = pNext;
	info.pQueueCreateInfos = queue_create_info.data();
	info.queueCreateInfoCount = (uint32_t)queue_create_info.size();
	info.pEnabledFeatures = &physical_device_features;
	info.enabledLayerCount = 0,			// DEPRECATED
	info.ppEnabledLayerNames = NULL,    // DEPRECATED
	info.ppEnabledExtensionNames = device_extensions.data();
	info.enabledExtensionCount = (uint32_t)device_extensions.size();

	return info;
}

VkDeviceQueueCreateInfo Zodiac::Initializers::DeviceQueueCreate(uint32_t queue_family_index, float& priority) {
	VkDeviceQueueCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = queue_family_index;
	info.queueCount = 1;
	info.pQueuePriorities = &priority;

	return info;
}

VkSwapchainCreateInfoKHR Zodiac::Initializers::SwapchainCreateInfo(VkSwapchainKHR& swapchain, VkSurfaceKHR& surface, VkPresentModeKHR& presentMode, SurfaceDetails* details, VkImageUsageFlags usageFlags, VkSurfaceFormatKHR surfaceFormat, VkExtent2D extent)
{
	VkSwapchainCreateInfoKHR info = {};

	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.pNext = NULL;
	info.surface = surface;
	info.minImageCount = details->capabilities.minImageCount + 1; //According to comments on swapchain extension
	info.imageFormat = surfaceFormat.format;
	info.imageColorSpace = surfaceFormat.colorSpace;
	info.imageExtent = extent;
	info.imageUsage = usageFlags;
	info.preTransform = details->capabilities.currentTransform;
	info.imageArrayLayers = 1;
	info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.queueFamilyIndexCount = 0;
	info.pQueueFamilyIndices = NULL;
	info.presentMode = presentMode;
	info.oldSwapchain = swapchain;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.clipped = VK_TRUE;

	return info;
}

VkImageViewCreateInfo Zodiac::Initializers::ImageViewCreateInfo(VkFormat colorFormat, VkImage image, VkImageSubresourceRange subResourceRange, VkImageViewType viewType)
{
	VkImageViewCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext = NULL;
	info.format = colorFormat;
	info.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	info.subresourceRange = subResourceRange;
	info.viewType = viewType;
	info.image = image;

	return info;
}

VkImageCreateInfo Zodiac::Initializers::ImageCreateInfo(VkFormat format, VkExtent3D extent, VkImageUsageFlags usageFlags, VkImageType imageType) {
	VkImageCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.imageType = imageType;
	info.format = format;
	info.extent = extent;
	info.mipLevels = 1;
	info.arrayLayers = 1;
	info.samples = VK_SAMPLE_COUNT_1_BIT;
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.usage = usageFlags;
	info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	return info;
}

VkRenderPassCreateInfo Zodiac::Initializers::RenderPassCreateInfo(std::vector<VkAttachmentDescription>& attachDesc, std::vector<VkSubpassDescription>& subpassDesc, std::vector<VkSubpassDependency>& dependencies) {
	VkRenderPassCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = static_cast<uint32_t>(attachDesc.size());
	info.pAttachments = attachDesc.data();
	info.subpassCount = static_cast<uint32_t>(subpassDesc.size());
	info.pSubpasses = subpassDesc.data();
	info.dependencyCount = static_cast<uint32_t>(dependencies.size());
	info.pDependencies = dependencies.data();

	return info;
}

VkPipelineCacheCreateInfo Zodiac::Initializers::PipelineCacheCreateInfo() {
	VkPipelineCacheCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

	return info;
}

VkPipelineShaderStageCreateInfo Zodiac::Initializers::PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule) {
	VkPipelineShaderStageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.flags = 0;
	info.stage = stage;
	info.module = shaderModule;
	info.pName = "main";
	info.pSpecializationInfo = nullptr;

	return info;
}

VkFramebufferCreateInfo Zodiac::Initializers::FramebufferCreateInfo(VkRenderPass renderPass, std::vector<VkImageView>& attachments, uint32_t width, uint32_t height) {
	VkFramebufferCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.renderPass = renderPass;
	info.attachmentCount = static_cast<uint32_t>(attachments.size());
	info.pAttachments = attachments.data();
	info.width = width;
	info.height = height;
	info.layers = 1;

	return info;
}

VkSemaphoreCreateInfo Zodiac::Initializers::SemaphoreCreateInfo()
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	return info;
}

VkFenceCreateInfo Zodiac::Initializers::FenceCreateInfo(VkFenceCreateFlags flags) {
	VkFenceCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.flags = flags;

	return info;
}

VkSubmitInfo Zodiac::Initializers::SubmitInfo(VkSemaphore& present_semaphore, VkSemaphore& render_semaphore, VkCommandBuffer& commandBuffer, VkPipelineStageFlags flags)
{
	VkSubmitInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.pWaitDstStageMask = &flags;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &present_semaphore;
	info.signalSemaphoreCount = 1;
	info.pSignalSemaphores = &render_semaphore;
	info.pCommandBuffers = &commandBuffer;
	info.commandBufferCount = 1;

	return info;
}

VkSubmitInfo Zodiac::Initializers::SubmitInfo(VkCommandBuffer& commandBuffer) {
	VkSubmitInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	info.pCommandBuffers = &commandBuffer;
	info.commandBufferCount = 1;
	return info;
}

VkCommandPoolCreateInfo Zodiac::Initializers::CommandPoolCreateinfo(uint32_t queue_family_index, VkCommandPoolCreateFlags flags) {
	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.queueFamilyIndex = queue_family_index;
	info.flags = flags;

	return info;
}

VkCommandBufferAllocateInfo Zodiac::Initializers::CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count) {
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	return info;
}

VkBufferCreateInfo Zodiac::Initializers::BufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage_flags) {
	VkBufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.size = size;
	info.usage = usage_flags;

	return info;
}

VkMemoryAllocateInfo Zodiac::Initializers::MemoryAllocateInfo(VkDeviceSize size, uint32_t memory_type_index) {
	VkMemoryAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = size;
	info.memoryTypeIndex = memory_type_index;

	return info;
}

VkShaderModuleCreateInfo Zodiac::Initializers::ShaderModuleCreateInfo(std::vector<char>& code) {
	VkShaderModuleCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.codeSize = code.size();
	info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	return info;
}

VkPipelineVertexInputStateCreateInfo Zodiac::Initializers::PipelineVertexInputStateCreateInfo(std::vector<VkVertexInputBindingDescription>& vertexInputBindingDesc, std::vector<VkVertexInputAttributeDescription>& vertexInputAttrDesc) {
	VkPipelineVertexInputStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindingDesc.size());
	info.pVertexBindingDescriptions = vertexInputBindingDesc.data();
	info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttrDesc.size());
	info.pVertexAttributeDescriptions = vertexInputAttrDesc.data();

	return info;
}

VkPipelineInputAssemblyStateCreateInfo Zodiac::Initializers::PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology) {
	VkPipelineInputAssemblyStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	//info.primitiveRestartEnable = VK_FALSE;
	info.topology = topology;

	return info;
}

VkPipelineViewportStateCreateInfo Zodiac::Initializers::PipelineViewportStateCreateInfo() {
	VkPipelineViewportStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.viewportCount = 1;
	//info.pViewports = &viewport;
	info.scissorCount = 1;
	//info.pScissors = &scissor;

	return info;
}

VkPipelineRasterizationStateCreateInfo Zodiac::Initializers::PipelineRasterizationStateCreateInfo() {
	VkPipelineRasterizationStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.depthClampEnable = VK_FALSE;
	info.rasterizerDiscardEnable = VK_FALSE;
	info.polygonMode = VK_POLYGON_MODE_FILL;
	info.cullMode = VK_CULL_MODE_NONE;//VK_CULL_MODE_BACK_BIT; //TODO: Turn on back face culling
	info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	info.depthBiasEnable = VK_FALSE;
	//info.depthBiasConstantFactor = 0.0f;
	//info.depthBiasClamp = 0.0f;
	//info.depthBiasSlopeFactor = 0.0f;
	info.lineWidth = 1.0f;

	return info;
}

VkPipelineMultisampleStateCreateInfo Zodiac::Initializers::PipelineMultisampleStateCreateInfo() { //For now disabled
	VkPipelineMultisampleStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.sampleShadingEnable = VK_FALSE;
	info.minSampleShading = 1.0f;
	info.pSampleMask = nullptr;
	info.alphaToCoverageEnable = VK_FALSE;
	info.alphaToOneEnable = VK_FALSE;

	return info;
}

VkPipelineColorBlendStateCreateInfo Zodiac::Initializers::PipelineColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState& colorBlendAttachmentState, uint32_t attachmentCount) {
	VkPipelineColorBlendStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.pAttachments = &colorBlendAttachmentState;
	//info.logicOpEnable = VK_FALSE;
	//info.logicOp = VK_LOGIC_OP_COPY;
	info.attachmentCount = attachmentCount;
	//info.blendConstants[0] = 0.0f;
	//info.blendConstants[1] = 0.0f;
	//info.blendConstants[2] = 0.0f;
	//info.blendConstants[3] = 0.0f;

	return info;
}

VkPipelineDynamicStateCreateInfo Zodiac::Initializers::PipelineDynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStates) {
	VkPipelineDynamicStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.pDynamicStates = dynamicStates.data();
	info.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());

	return info;
}

VkPipelineDepthStencilStateCreateInfo Zodiac::Initializers::PipelineDepthStencilStateCreateinfo() {
	VkPipelineDepthStencilStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.depthTestEnable = VK_TRUE;
	info.depthWriteEnable = VK_TRUE;
	info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	info.depthBoundsTestEnable = VK_FALSE;
	info.back.failOp = VK_STENCIL_OP_KEEP;
	info.back.passOp = VK_STENCIL_OP_KEEP;
	info.back.compareOp = VK_COMPARE_OP_ALWAYS;
	info.stencilTestEnable = VK_FALSE;
	info.front = info.back;

	return info;
}

VkGraphicsPipelineCreateInfo Zodiac::Initializers::GraphicsPipelineCreateInfo(std::vector<VkPipelineShaderStageCreateInfo>& shaderStageCreateInfos, VkPipelineVertexInputStateCreateInfo& vertexInputStageCreateInfo, VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo, VkPipelineViewportStateCreateInfo& viewportStateCreateInfo, VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo, VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo, VkPipelineColorBlendStateCreateInfo& colorBlendStateCreateInfo, VkPipelineDynamicStateCreateInfo& dynamicStateCreateInfo, VkPipelineLayout& pipelineLayout, VkRenderPass& renderPass, VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo) {
	VkGraphicsPipelineCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
	info.pStages = shaderStageCreateInfos.data();
	info.pVertexInputState = &vertexInputStageCreateInfo;
	info.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	info.pTessellationState = nullptr; //Right now, no tesselation
	info.pViewportState = &viewportStateCreateInfo;
	info.pRasterizationState = &rasterizationStateCreateInfo;
	info.pMultisampleState = &multisampleStateCreateInfo;
	info.pDepthStencilState = &depthStencilStateCreateInfo;
	info.pColorBlendState = &colorBlendStateCreateInfo;
	info.pDynamicState = &dynamicStateCreateInfo;
	info.layout = pipelineLayout;
	info.renderPass = renderPass;
	info.subpass = 0;
	//info.basePipelineHandle = VK_NULL_HANDLE;
	//info.basePipelineIndex = -1;

	return info;
}

VkPipelineLayoutCreateInfo Zodiac::Initializers::PipelineLayoutCreateInfo() {
	VkPipelineLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.pPushConstantRanges = nullptr;
	info.pSetLayouts = nullptr;
	info.pushConstantRangeCount = 0;
	info.setLayoutCount = 0;

	return info;
}

VkPipelineLayoutCreateInfo Zodiac::Initializers::PipelineLayoutCreateInfo(uint32_t layoutCount, VkDescriptorSetLayout& pipelineLayoutInfo) {
	VkPipelineLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.flags = 0;
	info.pNext = nullptr;
	info.pPushConstantRanges = nullptr;
	info.pSetLayouts = &pipelineLayoutInfo;
	info.pushConstantRangeCount = 0;
	info.setLayoutCount = layoutCount;

	return info;
}

VkPresentInfoKHR Zodiac::Initializers::PresentInfo(VkSwapchainKHR& swapchain, uint32_t& index, VkSemaphore& waitSemaphore) {
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.pNext = NULL;
	info.swapchainCount = 1;
	info.pSwapchains = &swapchain;
	info.pImageIndices = &index;
	
	//if (waitSemaphore != VK_NULL_HANDLE) {
		info.pWaitSemaphores = &waitSemaphore;
		info.waitSemaphoreCount = 1;
	//}
		
	info.pResults = nullptr;

	return info;
}

VkDescriptorSetLayoutCreateInfo Zodiac::Initializers::DescriptorSetLayoutCreateInfo(uint32_t bindingCount, VkDescriptorSetLayoutBinding& layoutBinding) {
	VkDescriptorSetLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = 0;
	info.bindingCount = bindingCount;
	info.pBindings = &layoutBinding;

	return info;
}

VkDescriptorPoolCreateInfo Zodiac::Initializers::DescriptorPoolCreateInfo(uint32_t count, VkDescriptorPoolSize* poolSizes, uint32_t maxSets, VkDescriptorPoolCreateFlags flags) {
	VkDescriptorPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	info.poolSizeCount = count;
	info.pPoolSizes = poolSizes;
	info.maxSets = maxSets;

	return info;
}

VkDescriptorSetAllocateInfo Zodiac::Initializers::DescriptorSetAllocateInfo(VkDescriptorPool* descriptorPool, uint32_t setCount, VkDescriptorSetLayout* setLayout) {
	VkDescriptorSetAllocateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.descriptorPool = *descriptorPool;
	info.descriptorSetCount = setCount;
	info.pSetLayouts = setLayout;

	return info;
}

VkDebugUtilsMessengerCreateInfoEXT Zodiac::Initializers::DebugUtilsMessengerCreateInfo() {
	VkDebugUtilsMessengerCreateInfoEXT info = {};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	info.pfnUserCallback = Zodiac::DebugCallback;
	info.pUserData = nullptr;

	return info;
}

VkCommandBufferBeginInfo Zodiac::Initializers::CommandBufferBeginInfo(VkCommandBufferUsageFlags flags) {
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags = flags;
	info.pInheritanceInfo = nullptr;

	return info;
}

VkRenderPassBeginInfo Zodiac::Initializers::RenderPassBeginInfo(VkRenderPass renderPass, VkExtent2D renderArea, VkClearValue* clearValue, uint32_t clearValueCount) {
	VkRenderPassBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.pNext = nullptr;
	info.renderPass = renderPass;
	info.renderArea.offset = { 0, 0 };
	info.renderArea.extent = renderArea;
	info.clearValueCount = clearValueCount;
	info.pClearValues = clearValue;

	return info;
}

VkVertexInputBindingDescription Zodiac::Initializers::VertexInputBindingDescription(uint32_t binding, uint32_t vertexSize) {
	VkVertexInputBindingDescription info = {};
	info.binding = binding;
	info.stride = vertexSize;
	info.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return info;
}

VkVertexInputAttributeDescription Zodiac::Initializers::VertexInputAttributeDescription(uint32_t location, uint32_t binding, uint32_t offset) {
	VkVertexInputAttributeDescription info = {};
	info.location = location;
	info.binding = binding;
	info.format = VK_FORMAT_R32G32B32_SFLOAT; //data type
	info.offset = offset; //byte offset

	return info;
}
