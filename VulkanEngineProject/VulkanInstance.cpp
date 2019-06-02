#include "VulkanInstance.h"

VulkanInstance::VulkanInstance(VulkanConfiguration& config) {
	VkApplicationInfo applicationInfo = Initializers::ApplicationInfo(config);
	VkInstanceCreateInfo instanceInfo = Initializers::InstanceCreateInfo(applicationInfo, this->layers, this->extensions);
	ErrorCheck(vkCreateInstance(&instanceInfo, NULL, &this->instance));
}

VulkanInstance::~VulkanInstance() {
	vkDestroyInstance(this->instance, NULL);
}

VkInstance& VulkanInstance::GetInstance() {
	return this->instance;
}
