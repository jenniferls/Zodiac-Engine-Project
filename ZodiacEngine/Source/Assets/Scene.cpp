#include <Zodiacpch.h>
#include "Scene.h"

Zodiac::Scene::Scene() {

}

Zodiac::Scene::~Scene() {

}

void Zodiac::Scene::Update(float dt) {
	float speed = 0.5f; // Temporary variable to control rotation speed
	for (uint32_t i = 0; i < m_flattenedMeshes.size(); i++) {
		m_flattenedMeshes[i].m_transform.rotation.y += speed * dt;
		//m_flattenedMeshes[i].m_transform.rotation.z += speed * dt;
		m_flattenedMeshes[i].PerFrameUpdate();
	}
}

void Zodiac::Scene::FlattenMeshes() {
	m_totalTriangleCount = 0;
	m_flattenedMeshes.clear();

	for (uint32_t i = 0; i < GetModelCount(); i++) {
		for (uint32_t j = 0; j < m_models[i].GetMeshCount(); j++) {
			m_flattenedMeshes.emplace_back(m_models[i].GetMesh(j));
			m_totalTriangleCount += m_models[i].GetMesh(j).GetIndexCount() / 3;
		}
	}
}

void Zodiac::Scene::AddModel(Model& model) {
	m_models.emplace_back(model);

	//for (uint32_t i = 0; i < model.GetMeshCount(); i++) {
	//	m_flattenedMeshes.emplace_back(model.GetMesh(i));
	//	m_totalTriangleCount += model.GetMesh(i).GetIndexCount() / 3;
	//}
	m_totalMeshCount += model.GetMeshCount();
	m_totalVertexCount += model.GetModelVertexCount();
}
