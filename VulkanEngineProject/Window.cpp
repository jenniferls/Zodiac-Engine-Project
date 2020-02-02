#include <Zodiacpch.h>
#include "Window.h"
#include "Validation.h"
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) //For detecting memory leaks

Zodiac::Window::Window(const WindowProperties& props) {
	Init(props);
}

Zodiac::Window::~Window() {
	Shutdown();
}

Zodiac::Window* Zodiac::Window::Create(const WindowProperties& props) {
	return new Window(props);
}

void Zodiac::Window::PollWindowEvents() {
	glfwPollEvents();
}

void* Zodiac::Window::GetNativeWindow() const {
	return m_window;
}

const char** Zodiac::Window::GetGLFWExtensions() const {
	return glfwExtensions;
}

uint32_t Zodiac::Window::GetGLFWExtCount() const {
	return glfwExtensionCount;
}

void Zodiac::Window::Init(const WindowProperties& props) {
	std::cout << ">>>> Initializing GLFW <<<<" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Not a resizable window
	if (!glfwVulkanSupported()) {
		throw std::runtime_error("GLFW ERROR: VULKAN IS NOT SUPPORTED!");
	}
	m_window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::cout << "GLFW initialization successful!" << std::endl;
}

void Zodiac::Window::Shutdown() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Zodiac::Window::WindowShouldClose() {
	return glfwWindowShouldClose(m_window);
}
