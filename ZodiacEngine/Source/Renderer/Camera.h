#pragma once
#include "CameraMovement.h"

namespace Zodiac {
	class Camera {
	public:
		Camera();
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const = 0;

		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetRotation() const { return m_rotation; }
		const glm::mat4& GetView() const { return m_view; }

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		void Update(float dt, const CameraMovement& movement);

	protected: 
		void RecalculateViewMatrix();

		glm::mat4 m_view = glm::mat4(1.0f);
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, -2.5f);
		glm::vec3 m_rotation = glm::vec3(0.0f);

		float m_speed = 5.0f;
		glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 m_orientation = glm::mat4(1.0f);
	};
}