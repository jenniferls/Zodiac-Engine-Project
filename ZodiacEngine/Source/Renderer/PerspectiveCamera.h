#pragma once
#include "Camera.h"

namespace Zodiac {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float FoV = 60.0f, float aspectRatio = 16.0f / 9.0f);
		~PerspectiveCamera();

	private:
		float m_fov;
		float m_aspectRatio;

		float m_yaw;
		float m_pitch;
		float m_roll;

		float m_nearClip;
		float m_farClip;

		glm::mat4 m_projMat;
		glm::mat4 m_viewMat;
		glm::mat4 m_modelMat;

		float m_camSpeed;
	};
}