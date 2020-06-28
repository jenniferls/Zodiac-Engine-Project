#pragma once
#include "vulkan/vulkan.h"

#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanBuffer.h"

namespace Zodiac {
	class Renderer {
	public:
		Renderer(const Renderer&) = delete;
		void operator=(Renderer const&) = delete;

		~Renderer();

		static void Init(VulkanDevice* device, Settings settings, VulkanSurface* surface);
		static Renderer& Get();

		static void BeginScene();
		static void Submit();
		static void EndScene();
		static void Flush();
		static void Shutdown();

		//void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

	private:
		Renderer();
		static void InitInternal();
		static void SetupRenderPass();
		static void SetupPipelineCache();
		static void SetupFramebuffers();

		static bool SetupPipeline();
		static void PrepareGeometry();

		static Renderer* s_instance;

		static VulkanDevice* s_device;
		static VulkanSurface* s_surface;
		static Settings s_settings;
		static VulkanSwapchain* s_swapchain;
		static VkRenderPass s_renderPass;
		static VkPipeline s_pipeline;
		static VkPipelineCache s_pipelineCache;
		static VkPipelineLayout s_pipelineLayout;
		static std::vector<VkFramebuffer> s_framebuffers;

		static std::vector<VkCommandBuffer> s_drawCmdBuffers;

		static VulkanSemaphore* s_presentSemaphore;
		static VulkanSemaphore* s_renderCompleteSemaphore;
		static std::vector<VulkanFence*> s_waitFences;
	};
}