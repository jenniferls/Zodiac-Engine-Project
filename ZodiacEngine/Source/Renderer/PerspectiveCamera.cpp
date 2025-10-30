#include <Zodiacpch.h>
#include "PerspectiveCamera.h"

Zodiac::PerspectiveCamera::PerspectiveCamera(float FoV, float aspectRatio) {
	m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_fov = FoV;
	m_aspectRatio = aspectRatio;

	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;

	m_camSpeed = 4.0f;
	
	m_nearClip = 0.1f;
	m_farClip = 100.0f;

	m_projMat = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
	m_viewMat = glm::lookAt(m_pos, m_pos + m_front, m_up);
	m_modelMat = glm::mat4(1.0f);
}

Zodiac::PerspectiveCamera::~PerspectiveCamera() {

}
