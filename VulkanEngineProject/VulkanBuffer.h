#pragma once
#include <vulkan\vulkan.h>

#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanBuffer {
	public:
		VulkanBuffer(VulkanDevice* device, void* ptr, unsigned int elementSize, unsigned int count = 1, VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		~VulkanBuffer();

		virtual void SetData();
		virtual void SetData(unsigned int count);
		virtual void SetData(unsigned int startIndex, unsigned int count);

		void MapMemory(); //TODO: This could be improved in several ways
		void UnmapMemory();

		void CopyFrom(VkCommandBuffer& cmdBuffer, VulkanBuffer* sourceBuffer);
		VkBuffer& GetBuffer();

		const VkDeviceSize GetTotalSize() const;
		VkDeviceMemory& GetDeviceMemory();

		//virtual void Resize();
		//virtual void Transfer();

	private:
		void CreateBuffer(VkMemoryPropertyFlags memoryFlags);
		void DestroyBuffer();
		void Flush();
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

		VkBufferCreateInfo m_bufferInfo;
	};
}