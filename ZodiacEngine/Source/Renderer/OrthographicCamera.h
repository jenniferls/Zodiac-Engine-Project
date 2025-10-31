#pragma once
#include "Camera.h"

namespace Zodiac {
	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearClip = -1.0f, float farClip = 1.0f);
		~OrthographicCamera();

		const glm::mat4& GetProjection() const override;

	private:
		float m_left = 0.0f;
		float m_right = 0.0f;
		float m_bottom = 0.0f;
		float m_top = 0.0f;
		float m_nearClip = -1.0f;
		float m_farClip = 1.0f;

		glm::mat4 m_projMat;
	};
}