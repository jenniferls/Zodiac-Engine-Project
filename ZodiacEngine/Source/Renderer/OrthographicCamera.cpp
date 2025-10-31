#include <Zodiacpch.h>
#include "OrthographicCamera.h"

Zodiac::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip) {
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_nearClip = nearClip;
	m_farClip = farClip;
}

Zodiac::OrthographicCamera::~OrthographicCamera() {

}

const glm::mat4& Zodiac::OrthographicCamera::GetProjection() const
{
	return glm::ortho<float>(m_left, m_right, m_bottom, m_top, m_nearClip, m_farClip);
}
