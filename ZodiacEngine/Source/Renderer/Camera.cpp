#include <Zodiacpch.h>
#include "Camera.h"

Zodiac::Camera::Camera()
{
	RecalculateViewMatrix();
}

const glm::mat4& Zodiac::Camera::GetView() const {
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -m_position);
	glm::mat4 rotation = glm::mat4_cast(m_orientation);

	glm::mat4 res = rotation * translation;

	return res;
	//return m_view;
}

void Zodiac::Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	RecalculateViewMatrix();
}

void Zodiac::Camera::SetRotation(const glm::vec3& rotation) {
	m_rotation = rotation;
	RecalculateViewMatrix();
}

void Zodiac::Camera::Update(float dt, const CameraMovement& movement, const MouseState& mouseState) {
	float mouseSpeed = 2.0f;

	glm::vec2 mousePos = glm::vec2(-mouseState.m_pos.x, mouseState.m_pos.y); //Invert X axis for easier navigation

	glm::vec2 deltaMousePos = mousePos - m_oldMousePos;
	if (mouseState.m_active) {
		glm::quat deltaQuat = glm::quat(glm::vec3(mouseSpeed * deltaMousePos.y, mouseSpeed * deltaMousePos.x, 0.0f));
		m_orientation = glm::normalize(deltaQuat * m_orientation);
	}
	m_oldMousePos = mousePos;

	// Basic camera movement implementation
	if (movement.Forward) {
		m_position += m_speed * dt * m_forward;
	}
	if (movement.Backward) {
		m_position -= m_speed * dt * m_forward;
	}
	if (movement.StrafeLeft) {
		m_position -= m_speed * dt * glm::normalize(glm::cross(m_up, m_forward));
	}
	if (movement.StrafeRight) {
		m_position += m_speed * dt * glm::normalize(glm::cross(m_up, m_forward));
	}
	if (movement.Up) { //Flipping Y axis for up and down to match left-handed coordinate system
		m_position -= m_speed * dt * m_up;
	}
	if (movement.Down) {
		m_position += m_speed * dt * m_up;
	}
	RecalculateViewMatrix();
}

void Zodiac::Camera::RecalculateViewMatrix() {
	//glm::vec3 direction;
	//direction.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
	//direction.y = sin(glm::radians(m_rotation.x));
	//direction.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));

	glm::mat4 View = GetView();

	m_forward = glm::vec3(View[0][2], View[1][2], View[2][2]);

	m_orientation = glm::lookAtLH(m_position, m_position + m_forward, m_up);
	m_view = glm::mat4_cast(m_orientation);
}
