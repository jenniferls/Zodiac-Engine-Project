#pragma once
#include <vulkan\vulkan.h>

#include "Validation.h"
#include "VulkanDevice.h"
#include "Initializers.h"

namespace Zodiac {
	class VulkanBuffer {
	public:
		VulkanBuffer(VulkanDevice* device, void* ptr, unsigned int elementSize, unsigned int count = 1, VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		~VulkanBuffer();

		virtual void SetData();
		virtual void SetData(unsigned int count);
		virtual void SetData(unsigned int startIndex, unsigned int count);

	private:
		void CreateBuffer();
		uint32_t FindMemoryType(VkMemoryPropertyFlags flags, uint32_t typeFilter);

		VulkanDevice* m_device;
		VkBuffer m_buffer;
		void* m_ptr;
		unsigned int m_elementSize;
		unsigned int m_count;
		VkDeviceSize m_totalSize;
		VkBufferUsageFlags m_usageFlags;
		VkDeviceMemory m_deviceMemory;
		void* m_mappedMemory;
	};
}