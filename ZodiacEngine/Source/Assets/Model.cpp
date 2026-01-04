#include <Zodiacpch.h>
#include "Model.h"

Zodiac::Model::Model() {

}

Zodiac::Model::~Model() {

}

void Zodiac::Model::AddMesh(){
	m_meshes.emplace_back(Mesh());
}

void Zodiac::Model::AddMesh(Mesh mesh) {
	m_meshes.emplace_back(std::move(mesh));
}
