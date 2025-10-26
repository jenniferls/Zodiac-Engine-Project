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

		Settings& GetSettings();

		void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

		VulkanSwapchain* m_swapchain = nullptr;
		bool m_prepared = false;
		bool m_showGui = true;

	private:
		Renderer() = default;
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

		Window* m_window = nullptr;

		VulkanDevice* m_device = nullptr;
		VulkanSurface* m_surface = nullptr;
		Settings m_settings;
		VkRenderPass m_renderPass;
		VkPipeline m_pipeline;
		VkPipelineCache m_pipelineCache;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorPool m_descriptorPool;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;
		static std::vector<VkFramebuffer> s_framebuffers;

		static std::vector<VkCommandBuffer> s_drawCmdBuffers;

		static std::vector<VulkanSemaphore*> s_presentSemaphores;
		static std::vector<VulkanSemaphore*> s_renderCompleteSemaphores;
		static std::vector<VulkanFence*> s_waitFences;
		static std::vector<VulkanFence*> s_imagesInFlight;
		size_t m_currentFrame = 0;

		VulkanBuffer* m_vertexBuffer = nullptr;
		VulkanBuffer* m_indexBuffer = nullptr;
		VulkanBuffer* m_uniformBuffer = nullptr;

		static VkClearValue s_clearValues[2];

		static std::unique_ptr<ImGuiLayer> s_imgui;

		bool m_swapchainDirty = false;
	};
}