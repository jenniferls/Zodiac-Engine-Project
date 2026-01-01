#pragma once
#include "Vertex.h"

namespace Zodiac {
	class Mesh {
	public:
		Mesh();
		~Mesh();

		void SetVertexBuffer(const void* vertices) { m_vertexBuffer = vertices; }
		void SetIndexBuffer(const std::vector<uint32_t>& indices) { m_indices = indices; }

	private:
		const void* m_vertexBuffer;
		std::vector<uint32_t> m_indices;
	};
}