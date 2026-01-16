#pragma once
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "imgui/imgui.h"

#include "Window.h"
#include "Camera.h"
#include "RenderContext.h"

namespace Zodiac {
	class ImGuiLayer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		static ImGuiLayer* Create();
		static void Shutdown();

		bool Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance);
		void UpdateGUI(RenderContext& renderContext);
		VkCommandBuffer PrepareCommandBuffer(uint32_t currentFrame, int image);

		bool HasBeenInitialized() const { return m_initialized; }

	private:
		static void SetupRenderPass(); //Not Used with dynamic rendering
		static void SetupDescriptorPool();

		static VulkanDevice* s_device;
		static VkDescriptorPool s_descriptorPool;
		static std::vector<VkCommandBuffer> s_command_buffers;

		VkDeviceMemory m_fontMemory = VK_NULL_HANDLE;
		VkImage m_fontImage = VK_NULL_HANDLE;
		VkImageView m_fontView = VK_NULL_HANDLE;

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;

		glm::vec4 m_clearColor = glm::vec4(0.2f, 0.0f, 0.2f, 1.0f);

		uint32_t m_minImageCount = 2; //Should be updated during main loop
		bool m_showDemoWindow = false;
		bool m_showAnotherWindow = false;
		bool m_initialized = false;
	};
}