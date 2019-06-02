#include "BUILD_ORDER.h"

VulkanInstance* instance;

int main() {
	VulkanConfiguration vulkan_config;
	vulkan_config.app_name = "Sandbox";
	vulkan_config.app_version = VK_MAKE_VERSION(0, 1, 0);

	instance = new VulkanInstance(vulkan_config);

	std::cout << "Hello world!" << std::endl;
	system("pause");

	delete instance;

	return 0;
}