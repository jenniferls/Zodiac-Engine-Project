#pragma once
#include "Vertex.h"
#include "Transform.h"

namespace Zodiac {
	struct MeshData {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<glm::vec3> colors;
	};

	class Mesh {
	public:
		Mesh();
		~Mesh();

		void PerFrameUpdate();

		void SetVertexBuffer(const std::vector<SimpleVertex>& vertices) { m_vertexBuffer = vertices; }
		void SetIndexBuffer(const std::vector<uint32_t>& indices) { m_indices = indices; }
		void SetModelMatrix(const glm::mat4& modelMatrix) { m_modelMatrix = modelMatrix; }

		std::vector<SimpleVertex>& GetVertexBuffer() { return m_vertexBuffer; }
		std::vector<uint32_t>& GetIndexBuffer() { return m_indices; }

		uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_vertexBuffer.size()); }
		uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_indices.size()); }
		const glm::mat4& GetModelMatrix() const { return m_modelMatrix; }
		const glm::mat3& GetNormalMatrix() const { return m_normalMatrix; }

		void AddVertex(const Vertex& vertexData);
		void AssembleVertexFromMeshDataAtIndex(uint32_t index);

		void AddIndex(uint32_t index) { m_indices.emplace_back(index); }

		void SetToTriangleMesh(); //Sets up the mesh as a simple triangle

		void SetPosition(const glm::vec3& position) { m_transform.position = position; }
		void SetRotation(const glm::vec3& rotation) { m_transform.rotation = rotation; }
		void SetScale(const glm::vec3& scale) { m_transform.scale = scale; }

		Transform m_transform;
		MeshData m_meshData;
	private:
		void CreateModelMatrix();
		void CreateNormalMatrix();

		std::string name = "Unnamed";

		std::vector<SimpleVertex> m_vertexBuffer;
		std::vector<uint32_t> m_indices;

		glm::mat4 m_modelMatrix = glm::mat4(1.0f);
		glm::mat3 m_normalMatrix = glm::mat3(1.0f);
	};
}