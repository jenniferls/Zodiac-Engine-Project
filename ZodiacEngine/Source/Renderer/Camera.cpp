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

void Zodiac::Camera::Update(float dt, const CameraMovement& movement)
{
	// Basic camera movement implementation
	if (movement.Forward) {
		m_position.z += m_speed * dt;
	}
	if (movement.Backward) {
		m_position.z -= m_speed * dt;
	}
	if (movement.StrafeLeft) {
		m_position.x -= m_speed * dt;
	}
	if (movement.StrafeRight) {
		m_position.x += m_speed * dt;
	}
	if (movement.Up) { //Flipping Y axis for up and down to match left-handed coordinate system
		m_position.y -= m_speed * dt;
	}
	if (movement.Down) {
		m_position.y += m_speed * dt;
	}
	RecalculateViewMatrix();
}

void Zodiac::Camera::RecalculateViewMatrix()
{
	//glm::vec3 direction;
	//direction.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
	//direction.y = sin(glm::radians(m_rotation.x));
	//direction.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));

	m_view = glm::lookAtLH(m_position, m_position + m_forward, m_up);
}
