#include <Zodiacpch.h>
#include "Window.h"
#include "Defines.h"
#include "Validation.h"

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

void Zodiac::Window::glfw_resize_callback(GLFWwindow*, int w, int h) {
	//TODO: Actually make this callback useful
	std::cout << "Window resized!" << std::endl;

	//g_SwapChainRebuild = true;
	//g_SwapChainResizeWidth = w;
	//g_SwapChainResizeHeight = h;
}

void Zodiac::Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "Spacebar pressed!" << std::endl;
	}
}

void Zodiac::Window::Init(const WindowProperties& props) {
	std::cout << ">>>> Initializing GLFW <<<<" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); //TODO: Make it truly resizable or put to VK_FALSE
	if (!glfwVulkanSupported()) {
		throw std::runtime_error("GLFW ERROR: VULKAN IS NOT SUPPORTED!");
	}
	m_window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	glfwSetFramebufferSizeCallback(m_window, glfw_resize_callback);
	glfwSetKeyCallback(m_window, glfw_key_callback);

	std::cout << "GLFW initialization successful!" << std::endl;
}

void Zodiac::Window::Shutdown() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Zodiac::Window::WindowShouldClose() {
	return glfwWindowShouldClose(m_window);
}
