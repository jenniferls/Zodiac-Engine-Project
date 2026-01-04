#pragma once
#include "Mesh.h"

namespace Zodiac {
	class Model {
	public:
		Model();
		~Model();

		void AddMesh();
		void AddMesh(Mesh mesh);
		
		const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
		uint32_t GetMeshCount() const { return static_cast<uint32_t>(m_meshes.size()); }

	private:
		std::vector<Mesh> m_meshes;
	};
}