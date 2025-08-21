#pragma once
#include <vulkan\vulkan.h>

#include "VulkanConfiguration.h"

namespace Zodiac {

	class VulkanInstance {
	public:
		VulkanInstance(VulkanConfiguration& config, const char** glfwExtensions, int glfwExtCount);
		~VulkanInstance();

		VkInstance& GetInstance();
		uint32_t GetInstanceVersion();

	private:
		void UpdateInstanceVersion();

		VkInstance m_instance;
		std::vector<const char*> m_layers;
		std::vector<const char*> m_extensions;

		struct {
			int Major = 0;
			int Minor = 0;
			int Patch = 0;
		} m_instanceVersion;

		VkDebugUtilsMessengerEXT m_debugMessenger;
	};
}