#include <Zodiacpch.h>
#include "Window.h"
#include "Renderer.h"
#include "Defines.h"
#include "Validation.h"

Zodiac::Window::Window(const WindowProperties& props) {
	Init(props);
	m_windowProps = props;
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

const Zodiac::WindowProperties& Zodiac::Window::GetWindowProperties() const
{
	return m_windowProps;
}

void Zodiac::Window::SetGLFWCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, glfw_resize_callback);
	glfwSetKeyCallback(m_window, glfw_key_callback);
	glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);
	glfwSetCursorPosCallback(m_window, glfw_cursor_position_callback);
}

void Zodiac::Window::glfw_resize_callback(GLFWwindow* window, int w, int h) {
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	win->m_renderer->SetFramebufferResized(true);
	win->m_windowProps.Width = w;
	win->m_windowProps.Height = h;

	std::cout << "Window resized!" << std::endl;
	std::cout << "New Width: " << w << " New Height: " << h << std::endl;
}

void Zodiac::Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	bool press = action != GLFW_RELEASE;

	switch (key) {
		case GLFW_KEY_SPACE:
			if (action == GLFW_PRESS) {
				std::cout << "Spacebar pressed!" << std::endl;
				win->m_renderer->ToggleImGui();
			}
			break;
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			break;
	}

	win->m_inputHandler->HandleCameraMovement(key, action, mods);
}

void Zodiac::Window::glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Zodiac::Window::glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void Zodiac::Window::Init(const WindowProperties& props) {
	std::cout << ">>>> Initializing GLFW <<<<" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	if (!glfwVulkanSupported()) {
		throw std::runtime_error("GLFW ERROR: VULKAN IS NOT SUPPORTED!");
	}
	m_window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	SetGLFWCallbacks();

	std::cout << "GLFW initialization successful!" << std::endl;
}

void Zodiac::Window::Shutdown() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Zodiac::Window::WaitEvents()
{
	glfwWaitEvents();
}

bool Zodiac::Window::WindowShouldClose() {
	return glfwWindowShouldClose(m_window);
}
