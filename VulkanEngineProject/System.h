#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

namespace Zodiac {
	class System {
	public:
		System(const char* applicationName);
		~System();

		bool Init();
		void Run();
		void Shutdown();

	private:
		void InitWindow();
		void ShutdownWindow();

		VulkanConfiguration m_vulkanConfig;
		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VulkanDevice* m_device;
		VulkanBuffer* m_buffer;

		GLFWwindow* m_window = nullptr;
		const uint32_t WINDOW_WIDTH = 1280;
		const uint32_t WINDOW_HEIGHT = 720;
	};
}