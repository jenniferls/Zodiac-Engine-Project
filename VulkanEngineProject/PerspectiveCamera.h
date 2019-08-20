#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

		glm::mat4 m_projMat;
		glm::mat4 m_viewMat;
		glm::mat4 m_modelMat;

		float m_camSpeed;
	};
}