#include "VulkanInstance.h"

Zodiac::VulkanInstance::VulkanInstance(VulkanConfiguration& config) {
	layers.push_back("VK_LAYER_KHRONOS_validation");
	layers.push_back("VK_LAYER_LUNARG_assistant_layer");
	//layers.push_back("VK_LAYER_LUNARG_api_dump"); //Detailed information about API calls, their parameters and values
	//layers.push_back("VK_LAYER_LUNARG_monitor"); //For printing fps to window title bar
	extensions.push_back("VK_EXT_debug_utils");

	VkApplicationInfo applicationInfo = Initializers::ApplicationInfo(config);
	VkInstanceCreateInfo instanceInfo = Initializers::InstanceCreateInfo(applicationInfo, this->layers, this->extensions);
	ErrorCheck(vkCreateInstance(&instanceInfo, NULL, &this->instance));
}

Zodiac::VulkanInstance::~VulkanInstance() {
	vkDestroyInstance(this->instance, NULL);
}

VkInstance& Zodiac::VulkanInstance::GetInstance() {
	return this->instance;
}
