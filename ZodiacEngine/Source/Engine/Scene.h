#pragma once
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