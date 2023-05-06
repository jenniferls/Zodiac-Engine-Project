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
		static void Shutdown();

		bool Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance);

	private:
		static void SetupRenderPass();
		static void SetupDescriptorPool();

		static VulkanDevice* s_device;
		static VkRenderPass s_renderPass;
		static VkDescriptorPool s_descriptorPool;

		VkSampler m_sampler;

		VulkanBuffer* m_vertexBuffer;
		VulkanBuffer* m_indexBuffer;

		VkDeviceMemory m_fontMemory = VK_NULL_HANDLE;
		VkImage m_fontImage = VK_NULL_HANDLE;
		VkImageView m_fontView = VK_NULL_HANDLE;

		VkPipelineCache m_pipelineCache;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;

		uint32_t m_minImageCount = 2; //Should be updated during main loop
	};
}