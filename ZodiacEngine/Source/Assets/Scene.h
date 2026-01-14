#pragma once
#include "Model.h"
#include <entt/entt.hpp>

namespace Zodiac {
	class Scene {
	public:
		Scene();
		~Scene();

		void Update(float dt);

		void FlattenMeshes(); //TODO: Find an improved solution later

		uint32_t GetModelCount() const { return static_cast<uint32_t>(m_models.size()); }
		void AddModel(Model& model);

		Model& GetModel(uint32_t index) { return m_models[index]; }
		std::vector<Model>& GetModels() { return m_models; }

		const std::vector<Mesh>& GetAllMeshesInScene() { return m_flattenedMeshes; }

		uint32_t GetSceneMeshCount() const { return m_totalMeshCount; }
		uint32_t GetSceneTriangleCount() const { return m_totalTriangleCount; }
		uint32_t GetSceneVertexCount() const { return m_totalVertexCount; }

	private:
		std::vector<Mesh> m_flattenedMeshes;

		std::vector<Model> m_models;
		entt::registry m_registry;
		uint32_t m_totalMeshCount = 0;
		uint32_t m_totalTriangleCount = 0;
		uint32_t m_totalVertexCount = 0;
	};
}