#include <Zodiacpch.h>
#include "MeshImporter.h"

//TODO: only compile assimp if needed
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Zodiac::MeshImporter::MeshImporter() {
}

Zodiac::MeshImporter::~MeshImporter(){
}

bool Zodiac::MeshImporter::LoadModel(const char* path, Mesh& mesh){
	//TODO: Extend to support binary format
	if (AssimpImport(path, mesh)) {
		return true;
	}
	else {
		return false;
	}
}

bool Zodiac::MeshImporter::AssimpImport(const char* path, Mesh& mesh){
	Assimp::Importer importer;
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FindDegenerates;

	const aiScene* scene = importer.ReadFile(path, flags);

	return false;
}
