#pragma once
#include "Vertex.h"

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

		void SetVertexBuffer(const std::vector<SimpleVertex>& vertices) { m_vertexBuffer = vertices; }
		void SetIndexBuffer(const std::vector<uint32_t>& indices) { m_indices = indices; }

		std::vector<SimpleVertex>& GetVertexBuffer() { return m_vertexBuffer; }
		std::vector<uint32_t>& GetIndexBuffer() { return m_indices; }

		uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_vertexBuffer.size()); }
		uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_indices.size()); }

		void AddVertex(const Vertex& vertexData);
		void AssembleVertexFromMeshDataAtIndex(uint32_t index);

		void AddIndex(uint32_t index) { m_indices.emplace_back(index); }

		MeshData m_meshData;

	private:
		std::vector<SimpleVertex> m_vertexBuffer;
		std::vector<uint32_t> m_indices;
	};
}