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

bool Zodiac::MeshImporter::LoadModel(const char* path, Model& model){
	//TODO: Extend to support binary format
	if (AssimpImport(path, model)) {
		return true;
	}
	else {
		return false;
	}
}

bool Zodiac::MeshImporter::AssimpImport(const char* path, Model& model){
	Assimp::Importer importer;
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_FindDegenerates | aiProcess_SortByPType;

	const aiScene* scene = importer.ReadFile(path, flags);

	if (scene) {
		if(scene->HasMeshes()) {
			for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
				Mesh outMesh;
				const aiMesh* mesh = scene->mMeshes[i];
				const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				if (!mesh->HasFaces()) {
					//Skip meshes without faces
					continue;
				}

				for (int j = 0; j < mesh->mNumVertices; j++) {
					outMesh.m_meshData.positions.emplace_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
					outMesh.m_meshData.normals.emplace_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
					if (mesh->HasTextureCoords(0)) {
						outMesh.m_meshData.uvs.emplace_back(glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y)); //Only first set of texture coordinates are relevant in this case
					}
					else {
						outMesh.m_meshData.uvs.emplace_back(glm::vec2(0.0f, 0.0f));
					}

					if (mesh->HasTangentsAndBitangents()) {
						outMesh.m_meshData.tangents.emplace_back(glm::vec3(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z));
						outMesh.m_meshData.bitangents.emplace_back(glm::vec3(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z));
					}
					else {
						outMesh.m_meshData.tangents.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
						outMesh.m_meshData.bitangents.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
					}

					if (mesh->HasVertexColors(0)) {
						outMesh.m_meshData.colors.emplace_back(glm::vec3(mesh->mColors[0][j].r, mesh->mColors[0][j].g, mesh->mColors[0][j].b));
					}
					else {
						outMesh.m_meshData.colors.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
					}

					outMesh.AssembleVertexFromMeshDataAtIndex(j);
				}

				for (int j = 0; j < mesh->mNumFaces; j++) {
					if (mesh->mFaces[j].mNumIndices == 3) {
						outMesh.AddIndex(mesh->mFaces[j].mIndices[0]);
						outMesh.AddIndex(mesh->mFaces[j].mIndices[1]);
						outMesh.AddIndex(mesh->mFaces[j].mIndices[2]);
					}
					else {
						std::cout << "ASSIMP: Model-loading failure! Mesh is not triangulated." << std::endl;
						std::cout << "Number of indices in face was: " << mesh->mFaces[j].mNumIndices << std::endl;
						return false;
					}
				}

				model.AddMesh(outMesh);
			}
		}
		if(scene->HasAnimations()) {
			//TODO: Process animations
			if (scene->mAnimations[0]->mTicksPerSecond != 0.0f) { //Get the framerate for the animation
				//model.animation.ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond;
				//model.animation.duration = scene->mAnimations[0]->mDuration;
			}
			else {
				//model.animation.ticksPerSecond = 30.0f; //If no framerate is found, default to 30
			}
		}
		if (scene->HasSkeletons()) {
			//TODO: Process skeletons
		}
		if(scene->HasMaterials()) {
			//TODO: Process materials
		}
	}
	else {
		std::cout << "ASSIMP: Model-loading failure! " << importer.GetErrorString() << std::endl;
		return false;
	}

	return true;
}
