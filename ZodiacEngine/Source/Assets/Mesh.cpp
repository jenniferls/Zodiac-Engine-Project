#include <Zodiacpch.h>
#include "Mesh.h"

Zodiac::Mesh::Mesh() {

}

Zodiac::Mesh::~Mesh() {

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
