#pragma once
#include "Scene.h"

namespace Zodiac {
	class Entity {
	public:
		Entity(Scene* scene);
		~Entity();

	private:
		Scene* m_scene = nullptr;
	};
}