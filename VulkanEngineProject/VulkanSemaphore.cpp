#include "VulkanSemaphore.h"

Zodiac::VulkanSemaphore::VulkanSemaphore(VulkanDevice* device)
{
	m_device = device;
	//VkSemaphoreCreateInfo semaphore_create_info = Initializers::SemaphoreCreateInfo();

	//ErrorCheck(vkCreateSemaphore(*device->GetDevice(), &semaphore_create_info, nullptr, &m_semaphore));
}

Zodiac::VulkanSemaphore::~VulkanSemaphore()
{
	vkDestroySemaphore(*m_device->GetDevice(), m_semaphore, nullptr);
}

VkSemaphore& Zodiac::VulkanSemaphore::GetSemaphore()
{
	return m_semaphore;
}