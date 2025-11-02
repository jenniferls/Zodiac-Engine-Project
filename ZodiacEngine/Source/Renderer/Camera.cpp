#include <Zodiacpch.h>
#include "Camera.h"

Zodiac::Camera::Camera()
{
	RecalculateViewMatrix();
}

void Zodiac::Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	RecalculateViewMatrix();
}

void Zodiac::Camera::SetRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
	RecalculateViewMatrix();
}

void Zodiac::Camera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	m_view = transform;
}
