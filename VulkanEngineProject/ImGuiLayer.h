#pragma once
#include <vulkan/vulkan.h>
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
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
		void UpdateGUI();
		VkCommandBuffer PrepareCommandBuffer(int image);

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

		glm::vec4 m_clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

		uint32_t m_minImageCount = 2; //Should be updated during main loop
		bool m_showDemoWindow = false;
		bool m_showAnotherWindow = false;
		bool m_swapchainRebuild = false;
		bool m_vSync = false;
	};
}