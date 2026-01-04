#pragma once
#include "Mesh.h"

namespace Zodiac {
	class MeshImporter {
	public:
		MeshImporter();
		~MeshImporter();

		bool LoadModel(const char* path, Mesh& mesh);

	private:
		bool AssimpImport(const char* path, Mesh& mesh);
		bool BinaryImport(const char* path, Mesh& mesh);
	};
}