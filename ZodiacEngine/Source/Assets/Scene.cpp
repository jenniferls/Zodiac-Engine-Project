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
		m_flattenedMeshes[i].PerFrameUpdate();
	}
	m_flattenedMeshes[1].SetPosition(glm::vec3(1.0f, -0.4f, 0.f));
	m_flattenedMeshes[1].SetScale(glm::vec3(0.5f));
}

void Zodiac::Scene::AddModel(Model& model) {
	m_models.emplace_back(model);

	for (uint32_t i = 0; i < model.GetMeshCount(); i++) {
		m_flattenedMeshes.emplace_back(model.GetMesh(i));
		m_totalTriangleCount += model.GetMesh(i).GetIndexCount() / 3;
	}
	m_totalMeshCount += model.GetMeshCount();
	m_totalVertexCount += model.GetModelVertexCount();
}
