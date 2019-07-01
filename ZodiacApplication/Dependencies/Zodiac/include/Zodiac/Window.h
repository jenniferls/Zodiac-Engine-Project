#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

namespace Zodiac {
	struct WindowProperties {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Zodiac Engine", unsigned int width = 1280, unsigned int height = 720) : Title(title), Width(width), Height(height) {

		}
	};

	class Window {
	public:
		Window(const WindowProperties& props);
		~Window();

		static Window* Create(const WindowProperties& props = WindowProperties());
		bool WindowShouldClose();
		void PollWindowEvents();
		void Shutdown();

		void* GetNativeWindow() const;

	private:
		void Init(const WindowProperties& props);

		GLFWwindow* m_window = nullptr;
		bool windowShouldClose = false;
	};
}