#include <Zodiacpch.h>
#include "VulkanFence.h"
#include "Initializers.h"
#include "Validation.h"

Zodiac::VulkanFence::VulkanFence(VulkanDevice* device) {
	m_device = device->GetDevice();
	VkFenceCreateInfo createInfo = Initializers::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	ErrorCheck(vkCreateFence(*device->GetDevice(), &createInfo, nullptr, &p_fence));
}

Zodiac::VulkanFence::~VulkanFence() {
	vkDestroyFence(*m_device, p_fence, nullptr);
}
