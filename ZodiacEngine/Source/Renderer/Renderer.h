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

		void Init(VulkanDevice* device, Settings settings, VulkanSurface* surface, VulkanInstance* instance, Window* window);
		static Renderer& Get();

		void Draw();

		void BeginDynamicRendering(VkCommandBuffer commandBuffer, int imageIndex, VkClearValue* pClearColor, VkClearValue* pDepthValue);

		void BeginScene();
		void Submit();
		void EndScene();
		void Flush();
		void Shutdown();

		void ToggleImGui();
		void SetFramebufferResized(bool resized);
		void SetSwapchainDirty();

		static Settings& GetSettings();

		void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

		static bool s_prepared;
		static VulkanSwapchain* s_swapchain;
		static bool s_showGui;

	private:
		Renderer();
		void InitInternal();
		void SetupRenderPass();
		void SetupPipelineCache();
		void SetupFramebuffers();
		void CreateSyncObjects();

		bool SetupPipeline();
		void PrepareGeometry();
		void PrepareUniformBuffers();
		void SetupDescriptorSets();
		void SetupDescriptorPool();
		void PrepareDescriptorSet();
		void BuildCommandBuffers();
		void AllocateCommandBuffers();
		void RecordCommandBuffer(int32_t index, bool secondBarrier = false);
		void RecreateSwapChain();
		void CleanupFramebuffers();
		void CleanupSwapchain();
		void CleanupSyncObjects();

		static const int MAX_FRAMES_IN_FLIGHT = 2;

		Window* m_window;

		VulkanDevice* m_device;
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