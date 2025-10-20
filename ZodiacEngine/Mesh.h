#pragma once
#include "Vertex.h"

namespace Zodiac {
	class Mesh {
	public:
		Mesh();
		~Mesh();

	private:
		std::vector<SimpleVertex> m_vertexBuffer;
	};
}