#pragma once
//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

#include "Settings.h"
#include "Window.h"

namespace Zodiac {
	class System {
	public:
		System(const char* applicationName);
		~System();

		bool Init();
		void Run();

	private:
		VulkanConfiguration m_vulkanConfig;
		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VulkanDevice* m_device;
		VulkanBuffer* m_buffer;

		std::unique_ptr<Window> m_window;
	};
}