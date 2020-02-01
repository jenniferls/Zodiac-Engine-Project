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
	if (!glfwVulkanSupported())
	{
		throw std::runtime_error("GLFW ERROR: VULKAN IS NOT SUPPORTED!");
	}
	m_window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::cout << "GLFW initialization successful!" << std::endl;
}

void Zodiac::Window::QuerySurfaceDetails(VulkanPhysicalDevice* physicalDevice)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &m_surfaceDetails.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->GetPhysicalDevice(), m_surface, &formatCount, m_surfaceDetails.supported_formats.data());
	if (formatCount != 0)
	{
		m_surfaceDetails.supported_formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->GetPhysicalDevice(), m_surface, &formatCount, m_surfaceDetails.supported_formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		m_surfaceDetails.supported_present_modes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetPhysicalDevice(), m_surface, &presentModeCount, m_surfaceDetails.supported_present_modes.data());
	}
}

void Zodiac::Window::Shutdown() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Zodiac::Window::CreateSurface(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice)
{
	ErrorCheck(glfwCreateWindowSurface(instance->GetInstance(), m_window, NULL, &m_surface));
	QuerySurfaceDetails(physicalDevice);

	return true;
}

void Zodiac::Window::DestroySurface(VulkanInstance* instance)
{
	vkDestroySurfaceKHR(instance->GetInstance(), m_surface, NULL);
}

bool Zodiac::Window::WindowShouldClose() {
	return glfwWindowShouldClose(m_window);
}
