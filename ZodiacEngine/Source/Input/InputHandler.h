#pragma once
#include <GLFW/glfw3.h>
#include "CameraMovement.h"

namespace Zodiac {
	class InputHandler {
	public:
		InputHandler();
		~InputHandler() = default;

		void SetCameraReference(CameraMovement& cameraMovement) { m_cameraMovement = cameraMovement; }

		CameraMovement& GetCameraMovement() { return m_cameraMovement; }
		bool HandleCameraMovement(int key, int action, int mods);

		void SetMousePosition(float x, float y, glm::vec2 windowSize);
		MouseState& GetMouseState() { return m_mouseState; }
		void SetMouseActiveState(bool active) { m_mouseState.m_active = active; }

	private:
		CameraMovement m_cameraMovement;
		MouseState m_mouseState;
	};
}