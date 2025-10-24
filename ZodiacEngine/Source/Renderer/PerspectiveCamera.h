#pragma once

namespace Zodiac {
	class PerspectiveCamera {
	public:
		PerspectiveCamera();
		~PerspectiveCamera();

	private:
		glm::vec3 m_pos;
		glm::vec3 m_front;
		glm::vec3 m_up;
		float m_fov;
		float m_yaw;
		float m_pitch;
		glm::vec3 m_worldUp;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;

		glm::mat4 m_projMat;
		glm::mat4 m_viewMat;
		glm::mat4 m_modelMat;

		float m_camSpeed;
	};
}