#pragma once
#include "vulkan/vulkan.h"

#include "ImGuiLayer.h"

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

		static void Init(VulkanDevice* device, Settings settings, VulkanSurface* surface, VulkanInstance* instance, Window* window);
		static Renderer& Get();

		static void Draw();

		static void BeginDynamicRendering(VkCommandBuffer commandBuffer, int imageIndex, VkClearValue* pClearColor, VkClearValue* pDepthValue);

		static void BeginScene();
		static void Submit();
		static void EndScene();
		static void Flush();
		static void Shutdown();

		static void ToggleImGui();
		static void SetFramebufferResized(bool resized);
		static void SetSwapchainDirty();

		static Settings& GetSettings();

		void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

		static bool s_prepared;
		static VulkanSwapchain* s_swapchain;
		static bool s_showGui;

	private:
		Renderer();
		static void InitInternal();
		static void SetupRenderPass();
		static void SetupPipelineCache();
		static void SetupFramebuffers();

		static bool SetupPipeline();
		static void PrepareGeometry();
		static void PrepareUniformBuffers();
		static void SetupDescriptorSets();
		static void SetupDescriptorPool();
		static void PrepareDescriptorSet();
		static void BuildCommandBuffers();
		static void AllocateCommandBuffers();
		static void RecordCommandBuffer(int32_t index, bool secondBarrier = false);
		static void RecreateSwapChain();
		static void CleanupFramebuffers();
		static void CleanupSwapchain();

		static const int MAX_FRAMES_IN_FLIGHT = 2;

		static Renderer* s_instance;
		static Window* s_window;

		static VulkanDevice* s_device;
		static VulkanSurface* s_surface;
		static Settings s_settings;
		static VkRenderPass s_renderPass;
		static VkPipeline s_pipeline;
		static VkPipelineCache s_pipelineCache;
		static VkPipelineLayout s_pipelineLayout;
		static VkDescriptorPool s_descriptorPool;
		static VkDescriptorSetLayout s_descriptorSetLayout;
		static VkDescriptorSet s_descriptorSet;
		static std::vector<VkFramebuffer> s_framebuffers;

		static std::vector<VkCommandBuffer> s_drawCmdBuffers;

		static std::vector<VulkanSemaphore*> s_presentSemaphores;
		static std::vector<VulkanSemaphore*> s_renderCompleteSemaphores;
		static std::vector<VulkanFence*> s_waitFences;
		static std::vector<VulkanFence*> s_imagesInFlight;
		static size_t s_currentFrame;

		static VulkanBuffer* s_vertexBuffer;
		static VulkanBuffer* s_indexBuffer;
		static VulkanBuffer* s_uniformBuffer;

		static VkClearValue s_clearValues[2];

		static std::unique_ptr<ImGuiLayer> s_imgui;

		static bool s_swapchainDirty;
	};
}