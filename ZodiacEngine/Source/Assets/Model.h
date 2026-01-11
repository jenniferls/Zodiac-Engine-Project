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
		Mesh& GetMesh(uint32_t index) { return m_meshes[index]; }
		uint32_t GetMeshCount() const { return static_cast<uint32_t>(m_meshes.size()); }

		uint32_t GetModelVertexCount() const { return m_totalVertexCount; }

		//void PerFrameUpdate();

		//const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }

		void SetPosition(const glm::vec3& position) { m_transform.position = position; }
		void SetRotation(const glm::vec3& rotation) { m_transform.rotation = rotation; }
		void SetScale(const glm::vec3& scale) { m_transform.scale = scale; }

		Transform m_transform;
	private:
		//void CreateModelMatrix();

		std::vector<Mesh> m_meshes;
		//glm::mat4 m_modelMatrix = glm::mat4();
		uint32_t m_totalVertexCount = 0;
		uint32_t m_totalIndexCount = 0;
	};
}