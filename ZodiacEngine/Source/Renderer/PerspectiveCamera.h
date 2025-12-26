#pragma once
#include "Camera.h"

namespace Zodiac {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float FoV = 60.0f, float aspectRatio = 16.0f / 9.0f, float nearClip = 0.1f, float farClip = 100.0f);
		~PerspectiveCamera();

		const glm::mat4& GetProjection() const override;

		float GetFoV() const { return m_fov; }
		void SetFoV(float fov);

	private:
		void RecalculateProjectionMatrix();

		float m_fov;
		float m_nearClip;
		float m_farClip;
		float m_aspectRatio;

		glm::mat4 m_projMat;
	};
}