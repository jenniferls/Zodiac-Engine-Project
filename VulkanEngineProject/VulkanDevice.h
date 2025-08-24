#pragma once
#include <vulkan\vulkan.h>

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

namespace Zodiac {
	class VulkanDevice {
	public:
		VulkanDevice(VulkanInstance* instance, VulkanPhysicalDevice* physical_device);
		~VulkanDevice();

		VkDevice* GetDevice();
		VkQueue* GetComputeQueue();
		VkQueue* GetGraphicsQueue();
		VkCommandPool& GetComputeCommandPool();
		VkCommandPool& GetGraphicsCommandPool();
		VulkanPhysicalDevice* GetPhysicalDevice();

		void GetComputeCommand(VkCommandBuffer* buffers, uint32_t count);
		void GetGraphicsCommand(VkCommandBuffer* buffers, uint32_t count);
		void SubmitGraphicsCommand(VkCommandBuffer* buffers);
		void FreeComputeCommand(VkCommandBuffer* buffers, uint32_t count);
		void FreeGraphicsCommand(VkCommandBuffer* buffers, uint32_t count);

		VkCommandBuffer GetCommandBuffer(bool begin); //Helper-function test
		void CreateCommandBuffers(int count, VkCommandBuffer* buffers);
		void FreeCommandBuffers(uint32_t count, const VkCommandBuffer* buffers);
		void FlushCommandBuffer(VkCommandBuffer& cmdBuffer, VkQueue& submitQueue, VkCommandPool& commandPool);

		bool IsExtensionSupported(const char* extensionName) const;

	private:
		std::vector<const char*> m_extensions;

		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VkDevice m_device;
		VkQueue m_compute_queue;
		VkQueue m_graphics_queue;
		VkCommandPool m_compute_command_pool;
		VkCommandPool m_graphics_command_pool;
		float m_priority;
	};
}