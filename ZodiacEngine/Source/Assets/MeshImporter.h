#pragma once
#include "Model.h"

namespace Zodiac {
	class MeshImporter {
	public:
		MeshImporter();
		~MeshImporter();

		bool LoadModel(const char* path, Model& model);

	private:
		bool AssimpImport(const char* path, Model& model);
		bool BinaryImport(const char* path, Model& model);
	};
}