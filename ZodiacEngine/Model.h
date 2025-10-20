#pragma once
#include "Mesh.h"

namespace Zodiac {
	class Model {
	public:
		Model();
		~Model();

		void AddMesh(Mesh& mesh);

	private:
		//std::vector<Mesh*> m_meshes;
	};
}