#include "InputHandler.h"
#pragma once

namespace Zodiac {
	Zodiac::InputHandler::InputHandler()
	{

	}

	bool InputHandler::HandleCameraMovement(int key, int action, int mods)
	{
        bool Press = action != GLFW_RELEASE;

        bool Handled = true;

        switch (key) {
        case GLFW_KEY_W:
            m_cameraMovement.Forward = Press;
            break;
        case GLFW_KEY_S:
            m_cameraMovement.Backward = Press;
            break;
        case GLFW_KEY_A:
            m_cameraMovement.StrafeLeft = Press;
            break;
        case GLFW_KEY_D:
            m_cameraMovement.StrafeRight = Press;
            break;
        case GLFW_KEY_PAGE_UP:
            m_cameraMovement.Up = Press;
            break;
        case GLFW_KEY_PAGE_DOWN:
            m_cameraMovement.Down = Press;
            break;
        case GLFW_KEY_KP_ADD:
            m_cameraMovement.Plus = Press;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            m_cameraMovement.Minus = Press;
            break;
        default:
            Handled = false;
        }

        return Handled;
	}
}