#pragma once
#include "Model.h"
//#include <entt/entt.hpp>

namespace Zodiac {
	struct MeshAlignmentData {
		uint32_t VertexBufferOffset = 0;
		uint32_t VertexBufferRange = 0;
		uint32_t IndexBufferOffset = 0;
		uint32_t IndexBufferRange = 0;
	};

	class Scene {
	public:
		Scene();
		~Scene();

		void Update();

		uint32_t GetModelCount() const { return static_cast<uint32_t>(m_models.size()); }
		void AddModel(Model& model);

		Model& GetModel(uint32_t index) { return m_models[index]; }
		std::vector<Model>& GetModels() { return m_models; }

		std::vector<const Mesh*> GetAllMeshesInScene() { return m_flattenedMeshes; }

		uint32_t GetSceneMeshCount() const { return m_totalMeshCount; }
		uint32_t GetSceneTriangleCount() const { return m_totalTriangleCount; }
		uint32_t GetSceneVertexCount() const { return m_totalVertexCount; }

		std::vector<MeshAlignmentData> m_meshAlignmentData;
	private:
		std::vector<const Mesh*> m_flattenedMeshes;

		std::vector<Model> m_models;
		//entt::registry m_registry;
		uint32_t m_totalMeshCount = 0;
		uint32_t m_totalTriangleCount = 0;
		uint32_t m_totalVertexCount = 0;
	};
}