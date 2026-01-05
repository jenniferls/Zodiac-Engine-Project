#include <Zodiacpch.h>
#include "Model.h"

Zodiac::Model::Model() {

}

Zodiac::Model::~Model() {

}

void Zodiac::Model::AddMesh() {
	m_meshes.emplace_back(Mesh());
}

void Zodiac::Model::AddMesh(Mesh mesh) {
	m_meshes.emplace_back(std::move(mesh));
}

void Zodiac::Model::PerFrameUpdate() {
	CreateModelMatrix();
}

void Zodiac::Model::CreateModelMatrix() {
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_transform.position);
	glm::mat4 rotationXMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationYMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_transform.scale);
	m_modelMatrix = translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;
}
