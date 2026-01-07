#pragma once
#include "Mesh.h"
#include "Transform.h"

namespace Zodiac {
	class Model {
	public:
		Model();
		~Model();

		void AddMesh();
		void AddMesh(Mesh mesh);
		
		const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
		uint32_t GetMeshCount() const { return static_cast<uint32_t>(m_meshes.size()); }

		uint32_t GetModelVertexCount() const { return m_totalVertexCount; }

		void PerFrameUpdate();

		const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }

		Transform m_transform;
	private:
		void CreateModelMatrix();

		std::vector<Mesh> m_meshes;
		glm::mat4 m_modelMatrix = glm::mat4();
		uint32_t m_totalVertexCount = 0;
	};
}