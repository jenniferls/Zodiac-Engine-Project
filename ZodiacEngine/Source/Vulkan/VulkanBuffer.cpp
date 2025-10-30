#include "Zodiacpch.h"
#include "VulkanBuffer.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanBuffer::VulkanBuffer(VulkanDevice* device, unsigned int elementSize, unsigned int count, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags) {
	m_device = device;
	m_elementSize = elementSize;
	m_count = count;
	m_totalSize = count * elementSize;
	m_usageFlags = usageFlags;
	CreateBuffer(memoryFlags);
}

Zodiac::VulkanBuffer::~VulkanBuffer() {
	//vkUnmapMemory(*m_device->GetDevice(), m_deviceMemory);
	vkDestroyBuffer(*m_device->GetDevice(), m_buffer, nullptr);
	vkFreeMemory(*m_device->GetDevice(), m_deviceMemory, nullptr);
}

void Zodiac::VulkanBuffer::SetData(void* ptr) {
	memcpy(m_mappedMemory, ptr, (unsigned int)m_totalSize);
}

void Zodiac::VulkanBuffer::SetData(void* ptr, unsigned int count) { //Should not be set to more data than originally allocated
	assert(count <= m_count && "Trying to set more data than initially allocated in buffer!");
	memcpy(m_mappedMemory, ptr, m_elementSize * count);
}

void Zodiac::VulkanBuffer::SetData(void* ptr, unsigned int startIndex, unsigned int count) { //Should not be set to more data than originally allocated
	memcpy(((char*)m_mappedMemory) + (startIndex * m_elementSize), ((char*)ptr) + (startIndex * m_elementSize), m_elementSize * count);
}

void Zodiac::VulkanBuffer::MapMemory() {
	ErrorCheck(vkMapMemory(*m_device->GetDevice(), m_deviceMemory, 0, m_mem_alloc_info.allocationSize, 0, &m_mappedMemory)); //Note to self: Keep an eye on the buffer size here to make sure this is correct usage
}

void Zodiac::VulkanBuffer::UnmapMemory() {
	vkUnmapMemory(*m_device->GetDevice(), m_deviceMemory);
}

void Zodiac::VulkanBuffer::CopyFrom(VkCommandBuffer& cmdBuffer, VulkanBuffer* sourceBuffer) {
	VkBufferCopy copyRegion = {};
	copyRegion.size = m_totalSize;
	vkCmdCopyBuffer(cmdBuffer, sourceBuffer->GetBuffer(), m_buffer, 1, &copyRegion);
}

VkBuffer& Zodiac::VulkanBuffer::GetBuffer() {
	return m_buffer;
}

unsigned int Zodiac::VulkanBuffer::GetCount(){
	return m_count;
}

const VkDeviceSize Zodiac::VulkanBuffer::GetTotalSize() const{
	return m_totalSize;
}

VkDeviceMemory& Zodiac::VulkanBuffer::GetDeviceMemory() {
	return m_deviceMemory;
}

void Zodiac::VulkanBuffer::CreateBuffer(VkMemoryPropertyFlags memoryFlags) {
	m_bufferInfo = Initializers::BufferCreateInfo(m_totalSize, m_usageFlags);
	ErrorCheck(vkCreateBuffer(*m_device->GetDevice(), &m_bufferInfo, nullptr, &m_buffer));

	VkMemoryRequirements mem_reqs;
	vkGetBufferMemoryRequirements(*m_device->GetDevice(), m_buffer, &mem_reqs);

	m_mem_alloc_info = Initializers::MemoryAllocateInfo(mem_reqs.size, FindMemoryType(memoryFlags, mem_reqs.memoryTypeBits));
	ErrorCheck(vkAllocateMemory(*m_device->GetDevice(), &m_mem_alloc_info, nullptr, &m_deviceMemory));
	ErrorCheck(vkBindBufferMemory(*m_device->GetDevice(), m_buffer, m_deviceMemory, 0)); //Last parameter is offset
}

uint32_t Zodiac::VulkanBuffer::FindMemoryType(VkMemoryPropertyFlags flags, uint32_t typeFilter) {
	for (uint32_t i = 0; i < m_device->GetPhysicalDevice()->GetDeviceMemoryProperties().memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (m_device->GetPhysicalDevice()->GetDeviceMemoryProperties().memoryTypes[i].propertyFlags & flags) == flags) {
			return i;
		}
	}
	assert(0 && "No available memory properties!");
	return -1;
}
