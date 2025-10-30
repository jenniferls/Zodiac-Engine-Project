#pragma once

namespace Zodiac {
	class Camera {
	public:
		Camera();
		virtual ~Camera() = default;

		glm::vec3 GetPosition() const { return m_pos; }
		void SetPosition(const glm::vec3& inPos) { m_pos = inPos; }

	protected: 
		glm::vec3 m_pos;
		glm::vec3 m_up;
		glm::vec3 m_front;

	private:

	};
}