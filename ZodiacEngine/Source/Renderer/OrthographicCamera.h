#pragma once
#include "Camera.h"

namespace Zodiac {
	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera();

		void setPosition(const glm::vec3& pos);
		void setRotation(float rotation);

		const glm::vec3& getPosition() const;
		const float getRotation() const;

		const glm::mat4& getProjectionMatrix() const;
		const glm::mat4& getViewMatrix() const;
		const glm::mat4& getModelMatrix() const;

	private:
		void RecalculateViewMatrix();

		glm::mat4 m_projMat;
		glm::mat4 m_viewMat;
		glm::mat4 m_modelMat;

		glm::vec3 m_position;
		float m_rotation;
	};
}