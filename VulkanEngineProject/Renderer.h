#pragma once
#include "vulkan/vulkan.h"

#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

namespace Zodiac {
	class Renderer {
	public:
		Renderer(const Renderer&) = delete;

		~Renderer();

		static void Init(VulkanDevice* device, Settings settings, VulkanSurface* surface);
		static Renderer& Get();

		static void BeginScene();
		static void Submit();
		static void EndScene();
		static void Flush();

		//void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

	private:
		Renderer();
		void Init();
		void SetupRenderPass();

		static Renderer* s_instance;

		static VulkanDevice* s_device;
		static VulkanSurface* s_surface;
		VulkanSwapchain* m_swapchain;
		static Settings s_settings;

		std::vector<VkCommandBuffer> m_drawCmdBuffers;
	};
}