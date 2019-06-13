#include "VulkanInstance.h"

Zodiac::VulkanInstance::VulkanInstance(VulkanConfiguration& config) {
	m_layers.push_back("VK_LAYER_KHRONOS_validation");
	m_layers.push_back("VK_LAYER_LUNARG_assistant_layer");
	//layers.push_back("VK_LAYER_LUNARG_api_dump"); //Detailed information about API calls, their parameters and values
	//layers.push_back("VK_LAYER_LUNARG_monitor"); //For printing fps to window title bar
	m_extensions.push_back("VK_EXT_debug_utils");

	VkApplicationInfo applicationInfo = Initializers::ApplicationInfo(config);
	VkInstanceCreateInfo instanceInfo = Initializers::InstanceCreateInfo(applicationInfo, m_layers, m_extensions);
	ErrorCheck(vkCreateInstance(&instanceInfo, NULL, &m_instance));
}

Zodiac::VulkanInstance::~VulkanInstance() {
	vkDestroyInstance(m_instance, NULL);
	m_instance = nullptr;
}

VkInstance& Zodiac::VulkanInstance::GetInstance() {
	return m_instance;
}
