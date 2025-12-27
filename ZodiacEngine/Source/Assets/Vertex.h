#pragma once
#include "glm/glm.hpp"

namespace Zodiac {
	struct SimpleVertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 uv;
	};

	struct Vertex {
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 color;
	};
}