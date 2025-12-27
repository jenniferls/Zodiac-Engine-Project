#pragma once
#include "vulkan/vulkan.h"

#include "ImGuiLayer.h"

#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanBuffer.h"

#include "Camera.h"
#include "FileWatcher.h"
#include "ShaderCompiler.h"

namespace Zodiac {
	struct PerFrameUniformData{
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix;
	};

	struct PipelineState {

	};

	class Renderer {
	public:
		Renderer(const Renderer&) = delete;
		void operator=(Renderer const&) = delete;

		~Renderer();

		void Init(VulkanDevice* device, Settings settings, VulkanSurface* surface, VulkanInstance* instance, Window* window, FileWatcher& filewatcher);
		static Renderer& Get();

		void Draw(float dt, Camera* mainCamera); //Delta time and camera for test purposes

		void BeginDynamicRendering(VkCommandBuffer commandBuffer, int imageIndex, VkClearValue* pClearColor, VkClearValue* pDepthValue);

		void BeginScene();
		void Submit();
		void EndScene();
		void Flush();
		void Shutdown();

		void ToggleImGui();
		bool IsImGuiActive() const { return m_showGui; }
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
		bool RecreatePipeline();
		void TraverseShaderVariableLayout(slang::VariableLayoutReflection* variableLayoutReflection);
		void SetupVertexBuffers();
		void PrepareUniformBuffers();
		void SetupDescriptorSets();
		void SetupDescriptorPool();
		void PrepareDescriptorSet();
		void BuildCommandBuffers();
		void AllocateCommandBuffers();
		void RecordCommandBuffer(int32_t index, bool secondBarrier = false);
		void RecreateSwapChain();
		void CleanupFramebuffers();
		void CleanupSyncObjects();

		void UpdateUniformBuffers(uint32_t currentImage, float dt, Camera* mainCamera); //Delta time for test purposes

		static const int MAX_FRAMES_IN_FLIGHT = 2;

		Window* m_window = nullptr;
		FileWatcher* m_fileWatcher = nullptr;

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
		std::vector<VkFramebuffer> m_framebuffers;

		std::vector<VkCommandBuffer> m_drawCmdBuffers;

		std::vector<VulkanSemaphore*> m_presentSemaphores;
		std::vector<VulkanSemaphore*> m_renderCompleteSemaphores;
		std::vector<VulkanFence*> m_waitFences;
		std::vector<VulkanFence*> m_imagesInFlight;
		size_t m_currentFrame = 0;

		VulkanBuffer* m_vertexBuffer = nullptr;
		VulkanBuffer* m_indexBuffer = nullptr;
		VulkanBuffer* m_uniformBuffer = nullptr;

		VkClearValue m_clearValues[2];

		std::unique_ptr<ImGuiLayer> m_imgui = nullptr;

		bool m_swapchainDirty = false;

		float testVal = 0.0f; //For testing updating uniform buffers
		PerFrameUniformData uboVS; //For testing updating uniform buffers
	};
}