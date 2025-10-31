#pragma once
#include <entt.hpp>
#include "PerspectiveCamera.h"

namespace Zodiac {
	class Scene {
	public:
		Scene();
		~Scene();

	private:
		PerspectiveCamera m_mainCamera;
	};
}