#include <Zodiacpch.h>
#include "Mesh.h"

Zodiac::Mesh::Mesh() {

}

Zodiac::Mesh::~Mesh() {

}

void Zodiac::Mesh::PerFrameUpdate() {
	CreateModelMatrix();
	CreateNormalMatrix();
}

void Zodiac::Mesh::AddVertex(const Vertex& vertexData){
	SimpleVertex vertex;
	vertex.pos = vertexData.pos;
	vertex.uv = vertexData.uv;
	vertex.color = vertexData.color;

	m_vertexBuffer.emplace_back(vertex);
}

void Zodiac::Mesh::AssembleVertexFromMeshDataAtIndex(uint32_t index){
	SimpleVertex vertex;
	vertex.pos = m_meshData.positions[index];
	vertex.uv = m_meshData.uvs[index];
	vertex.color = m_meshData.colors[index];
	vertex.normal = m_meshData.normals[index];

	m_vertexBuffer.emplace_back(vertex);
}

void Zodiac::Mesh::CreateModelMatrix() {
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_transform.position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, m_transform.scale);
}

void Zodiac::Mesh::CreateNormalMatrix() {
	m_normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_modelMatrix)));
}
