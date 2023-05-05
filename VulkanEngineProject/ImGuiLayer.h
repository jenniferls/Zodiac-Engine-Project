#pragma once
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"
#include "VulkanDevice.h"

#include "Window.h"

namespace Zodiac {
	class ImGuiLayer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		static ImGuiLayer* Create();

		bool Init(GLFWwindow* window, VulkanDevice* device);

	private:
		VkSampler m_sampler;

		VulkanBuffer* m_vertexBuffer;
		VulkanBuffer* m_indexBuffer;

		VulkanDevice* m_device;

		VkDeviceMemory m_fontMemory = VK_NULL_HANDLE;
		VkImage m_fontImage = VK_NULL_HANDLE;
		VkImageView m_fontView = VK_NULL_HANDLE;

		VkPipelineCache m_pipelineCache;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		VkDescriptorPool m_descriptorPool;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;
	};
}