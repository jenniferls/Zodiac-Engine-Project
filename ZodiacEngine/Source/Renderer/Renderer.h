#pragma once
#include "vulkan/vulkan.h"

#include "Scene.h"

#include "ImGuiLayer.h"

#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanBuffer.h"

#include "RenderContext.h"

#include "Camera.h"
#include "FileWatcher.h"
#include "ShaderCompiler.h"

namespace Zodiac {
	struct PerFrameUniformData {
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix;
		glm::mat3 normalMatrix;
	};

	struct MeshMetaData {
		uint32_t BaseIndex;
		uint32_t IndexCount;
		uint32_t BaseVertex;
	};

	struct PerInstanceData {
		glm::mat4 modelMatrix;
		glm::mat3 normalMatrix;
	};

	struct MeshAlignmentData {
		uint32_t VertexBufferOffset = 0;
		uint32_t VertexBufferRange = 0;
		uint32_t IndexBufferOffset = 0;
		uint32_t IndexBufferRange = 0;
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

		void Draw(float dt, RenderContext& renderContext); //Delta time and camera for test purposes

		void BeginDynamicRendering(VkCommandBuffer commandBuffer, int imageIndex, VkClearValue* pClearColor, VkClearValue* pDepthValue);

		//void BeginScene();
		//void Submit();
		//void EndScene();
		void Flush();
		void Shutdown();

		void ToggleImGui();
		bool IsImGuiActive() const { return m_showGui; }
		void SetFramebufferResized(bool resized);
		void SetSwapchainDirty();

		void PrepareForScene(Scene& scene); //TODO: refactor later

		Settings& GetSettings();

		void SetClearColor(const glm::vec4 color);
		void Clear();

		static const int MAX_FRAMES_IN_FLIGHT = 2;

		VulkanSwapchain* m_swapchain = nullptr;
		bool m_prepared = false;
		bool m_showGui = true;

	private:
		Renderer() = default;
		void SetupRenderPass();
		void SetupPipelineCache();
		void SetupFramebuffers();
		void CreateSyncObjects();

		bool SetupPipeline();
		bool RecreatePipeline();
		//void TraverseShaderVariableLayout(slang::VariableLayoutReflection* variableLayoutReflection);
		void PrepareUniformBuffers();

		void PrepareBuffersFromScene(Scene& scene);
		void SetupVertexBuffers(Scene& scene);
		void UpdateMeshAlignment(Scene& scene);
		void CreateMetaDataBuffer(Scene& scene);
		void CreateIndirectBuffer(Scene& scene);
		void CreatePerInstanceBuffer(const Scene& scene);

		void SetupDescriptorSets();
		void SetupDescriptorPool(uint32_t uniformBufferCount, uint32_t storageBufferCount, uint32_t maxSets);
		void PrepareDescriptorSet();
		void AllocateCommandBuffers();
		void RecordCommandBuffer(std::vector<VkCommandBuffer>& commandBuffers, uint32_t currentframe, int32_t index, RenderContext& renderContext, bool secondBarrier = false);
		void RecreateSwapChain();
		void CleanupFramebuffers();
		void CleanupSyncObjects();

		void UpdateUniformBuffers(uint32_t currentImage, float dt, Camera* mainCamera);
		void UpdatePerFrameData(uint32_t currentImage, float dt, RenderContext& renderContext); //Delta time for test purposes

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
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
		std::vector<std::vector<VkDescriptorSet>> m_descriptorSets; //Per swapchain image
		std::vector<VkFramebuffer> m_framebuffers;

		std::vector<VkCommandBuffer> m_drawCmdBuffers;

		std::vector<VulkanSemaphore*> m_presentSemaphores;
		std::vector<VulkanSemaphore*> m_renderCompleteSemaphores;
		std::vector<VulkanFence*> m_imagesInFlightFences;
		size_t m_currentFrame = 0;

		//Global buffers
		VulkanBuffer* m_vertexBuffer = nullptr;
		VulkanBuffer* m_indexBuffer = nullptr;
		std::vector<VulkanBuffer*> m_uniformBuffer;
		VulkanBuffer* m_metaDataBuffer = nullptr;
		VulkanBuffer* m_indirectBuffer = nullptr;
		std::vector<VulkanBuffer*> m_perInstanceBuffer;

		std::vector<MeshAlignmentData> m_meshAlignmentData;

		VkClearValue m_clearValues[2];

		std::unique_ptr<ImGuiLayer> m_imgui = nullptr;

		bool m_swapchainDirty = false;

		float testVal = 0.0f; //For testing updating uniform buffers
		PerFrameUniformData uboVS; //For testing updating uniform buffers
	};
}