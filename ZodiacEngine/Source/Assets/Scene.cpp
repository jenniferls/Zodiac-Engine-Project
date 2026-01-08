#include <Zodiacpch.h>
#include "Scene.h"

Zodiac::Scene::Scene() {

}

Zodiac::Scene::~Scene() {

}

void Zodiac::Scene::Update() {
	for (uint32_t i = 0; i < m_models.size(); i++) {
		m_models[i].PerFrameUpdate();
	}
}

void Zodiac::Scene::AddModel(Model& model) {
	m_models.emplace_back(model);
	for (uint32_t i = 0; i < model.GetMeshCount(); i++) {
		m_flattenedMeshes.emplace_back(&model.GetMesh(i));
		m_totalTriangleCount += model.GetMesh(i).GetIndexCount() / 3;
	}
	m_totalMeshCount += model.GetMeshCount();
	m_totalVertexCount += model.GetModelVertexCount();
}