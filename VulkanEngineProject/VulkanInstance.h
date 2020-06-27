#pragma once
#include <vulkan\vulkan.h>

#include "VulkanConfiguration.h"

namespace Zodiac {
	class VulkanInstance {
	public:
		VulkanInstance(VulkanConfiguration& config, const char** glfwExtensions, int glfwExtCount);
		~VulkanInstance();

		VkInstance& GetInstance();

	private:
		VkInstance m_instance;
		std::vector<const char*> m_layers;
		std::vector<const char*> m_extensions;

		VkDebugUtilsMessengerEXT m_debugMessenger;
	};
}