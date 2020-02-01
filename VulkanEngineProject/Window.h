#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "SurfaceDetails.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"

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

		bool CreateSurface(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice);
		void DestroySurface(VulkanInstance* instance);

		void* GetNativeWindow() const;
		const char** GetGLFWExtensions() const;
		uint32_t GetGLFWExtCount() const;

	private:
		void Init(const WindowProperties& props);
		void QuerySurfaceDetails(VulkanPhysicalDevice* physicalDevice);

		GLFWwindow* m_window = nullptr;

		VkSurfaceKHR m_surface;
		SurfaceDetails m_surfaceDetails;

		bool windowShouldClose = false;
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
	};
}