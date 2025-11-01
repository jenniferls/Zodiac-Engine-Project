#include <Zodiacpch.h>
#include "PerspectiveCamera.h"

Zodiac::PerspectiveCamera::PerspectiveCamera(float FoV, float aspectRatio, float nearClip, float farClip) {
	m_fov = FoV;
	m_aspectRatio = aspectRatio;
	m_nearClip = nearClip;
	m_farClip = farClip;
}

Zodiac::PerspectiveCamera::~PerspectiveCamera() {

}

const glm::mat4& Zodiac::PerspectiveCamera::GetProjection() const
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
}

void Zodiac::PerspectiveCamera::SetFoV(float fov)
{
	m_fov = fov;
	RecalculateProjectionMatrix();
}

void Zodiac::PerspectiveCamera::RecalculateProjectionMatrix()
{
	m_projMat = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
}
