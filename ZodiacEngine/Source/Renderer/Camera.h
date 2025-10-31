#pragma once

namespace Zodiac {
	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const = 0;

		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetRotation() const { return m_rotation; }
		const glm::mat4& GetView() const { return m_view; }

	protected: 
		glm::mat4 m_view = glm::mat4(1.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f);
	};
}