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

	private:
		CameraMovement m_cameraMovement;
	};
}