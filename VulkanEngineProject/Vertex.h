#pragma once
#include "glm/glm.hpp"

namespace Zodiac {
	struct SimpleVertex {
		glm::vec3 pos;
		glm::vec3 color;
	};

	struct Index {
		uint32_t index;
	};
}