#pragma once

namespace Zodiac {
	struct CameraMovement {
		bool Forward = false;
		bool Backward = false;
		bool StrafeLeft = false;
		bool StrafeRight = false;
		bool Up = false;
		bool Down = false;
		bool Plus = false;
		bool Minus = false;
	};

	struct MouseState {
		glm::vec2 m_pos = glm::vec2(0.0f);
		bool m_buttonPressed = false;
		bool m_active = false;
	};
}