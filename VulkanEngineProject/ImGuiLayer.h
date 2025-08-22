#pragma once
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "imgui/imgui.h"

#include "Window.h"

namespace Zodiac {
	class ImGuiLayer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		static ImGuiLayer* Create();
		static void Shutdown();

		bool Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance);
		void Render(Window* window, VulkanInstance* instance);

	private:
		static void SetupRenderPass();
		static void SetupDescriptorPool();

		static VulkanDevice* s_device;
		static VkRenderPass s_renderPass;
		static VkDescriptorPool s_descriptorPool;
		
		VkSampler m_sampler;

		VulkanBuffer* m_vertexBuffer;
		VulkanBuffer* m_indexBuffer;

		int m_framebufferWidth = 0;
		int m_framebufferHeight = 0;

		VkDeviceMemory m_fontMemory = VK_NULL_HANDLE;
		VkImage m_fontImage = VK_NULL_HANDLE;
		VkImageView m_fontView = VK_NULL_HANDLE;

		VkPipelineCache m_pipelineCache;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;

		glm::vec4 m_clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

		uint32_t m_minImageCount = 2; //Should be updated during main loop
		bool m_showDemoWindow = true;
		bool m_showAnotherWindow = true;
		bool m_swapchainRebuild = false;
	};
}