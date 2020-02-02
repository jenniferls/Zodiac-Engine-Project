#include "VulkanInstance.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanInstance::VulkanInstance(VulkanConfiguration& config, const char** glfwExtensions, int glfwExtCount) {
	m_layers.push_back("VK_LAYER_KHRONOS_validation");
	m_layers.push_back("VK_LAYER_LUNARG_assistant_layer");
	//m_layers.push_back("VK_LAYER_LUNARG_api_dump"); //Detailed information about API calls, their parameters and values
	//m_layers.push_back("VK_LAYER_LUNARG_monitor"); //For printing fps to window title bar
	m_extensions.push_back("VK_EXT_debug_utils");
	m_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME); //Instance extension

	for (int i = 0; i < glfwExtCount; i++) {
		m_extensions.push_back(glfwExtensions[i]);
	}

	VkApplicationInfo applicationInfo = Initializers::ApplicationInfo(config);
	VkInstanceCreateInfo instanceInfo = Initializers::InstanceCreateInfo(applicationInfo, m_layers, m_extensions);
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsInfo = Initializers::DebugUtilsMessengerCreateInfo();
	ErrorCheck(vkCreateInstance(&instanceInfo, NULL, &m_instance));
}

Zodiac::VulkanInstance::~VulkanInstance() {
	vkDestroyInstance(m_instance, NULL);
	m_instance = nullptr;
}

VkInstance& Zodiac::VulkanInstance::GetInstance() {
	return m_instance;
}
