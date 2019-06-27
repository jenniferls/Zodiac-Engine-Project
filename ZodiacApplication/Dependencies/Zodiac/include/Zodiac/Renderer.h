#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

namespace Zodiac {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		static void BeginScene();
		static void Submit();
		static void EndScene();
		static void Flush();

		void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

	private:

	};
}