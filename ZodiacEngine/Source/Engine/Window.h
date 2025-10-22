#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Zodiac {
	struct WindowProperties {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Zodiac Engine", unsigned int width = 1280, unsigned int height = 720) : Title(title), Width(width), Height(height) {

		}
	};

	class Renderer;

	class Window {
	public:
		Window(const WindowProperties& props);
		~Window();

		static Window* Create(const WindowProperties& props = WindowProperties());
		bool WindowShouldClose();
		void PollWindowEvents();
		void Shutdown();
		void WaitEvents();

		void* GetNativeWindow() const;
		const char** GetGLFWExtensions() const;
		uint32_t GetGLFWExtCount() const;
		const WindowProperties& GetWindowProperties() const;

		unsigned int GetWidth() const { return m_windowProps.Width; }
		unsigned int GetHeight() const { return m_windowProps.Height; }

		void SetRenderer(Renderer* renderer) { m_renderer = renderer; }

		static void glfw_resize_callback(GLFWwindow* window, int w, int h);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		void Init(const WindowProperties& props);

		GLFWwindow* m_window = nullptr;
		Renderer* m_renderer = nullptr;
		WindowProperties m_windowProps;

		bool windowShouldClose = false;
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
	};
}