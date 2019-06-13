#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice* device, void* ptr, unsigned int elementSize, unsigned int count, VkBufferUsageFlags usageFlags) {
	m_device = device;
	m_ptr = ptr;
	m_elementSize = elementSize;
	m_count = count;
	m_totalSize = m_count * m_elementSize;
	m_usageFlags = usageFlags;
	CreateBuffer();
}

VulkanBuffer::~VulkanBuffer() {
	vkUnmapMemory(*m_device->GetDevice(), m_deviceMemory);
	vkDestroyBuffer(*m_device->GetDevice(), m_buffer, nullptr);
	vkFreeMemory(*m_device->GetDevice(), m_deviceMemory, nullptr);
}

void VulkanBuffer::SetData() {
	memcpy(m_mappedMemory, m_ptr, (unsigned int)m_totalSize);
}

void VulkanBuffer::SetData(unsigned int count) {
	memcpy(m_mappedMemory, m_ptr, m_elementSize * count);
}

void VulkanBuffer::SetData(unsigned int startIndex, unsigned int count) {
	memcpy(((char*)m_mappedMemory) + (startIndex * m_elementSize), ((char*)m_ptr) + (startIndex * m_elementSize), m_elementSize * count);
}

void VulkanBuffer::CreateBuffer() {
	VkBufferCreateInfo buffer_info = Initializers::BufferCreateInfo(m_totalSize, m_usageFlags);
	ErrorCheck(vkCreateBuffer(*m_device->GetDevice(), &buffer_info, nullptr, &m_buffer));

	VkMemoryRequirements mem_reqs;
	vkGetBufferMemoryRequirements(*m_device->GetDevice(), m_buffer, &mem_reqs);

	VkMemoryAllocateInfo mem_alloc_info = Initializers::MemoryAllocateInfo(mem_reqs.size, FindMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, mem_reqs.memoryTypeBits));
	ErrorCheck(vkAllocateMemory(*m_device->GetDevice(), &mem_alloc_info, nullptr, &m_deviceMemory));
	ErrorCheck(vkBindBufferMemory(*m_device->GetDevice(), m_buffer, m_deviceMemory, 0)); //Last parameter is offset
	vkMapMemory(*m_device->GetDevice(), m_deviceMemory, 0, buffer_info.size, 0, &m_mappedMemory); //Note to self: Keep an eye on the buffer size here to make sure this is correct usage
}

uint32_t VulkanBuffer::FindMemoryType(VkMemoryPropertyFlags flags, uint32_t typeFilter) {
	for (uint32_t i = 0; i < m_device->GetPhysicalDevice()->GetDeviceMemoryProperties().memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (m_device->GetPhysicalDevice()->GetDeviceMemoryProperties().memoryTypes[i].propertyFlags & flags) == flags) {
			return i;
		}
	}
	assert(0 && "No available memory properties!");
	return -1;
}
