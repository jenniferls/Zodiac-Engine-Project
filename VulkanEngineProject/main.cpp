#include "BUILD_ORDER.h"

VulkanInstance* instance;

int main() {
	VulkanConfiguration vulkan_config;
	vulkan_config.app_name = "Sandbox";
	vulkan_config.app_version = VK_MAKE_VERSION(0, 1, 0);

	instance = new VulkanInstance(vulkan_config);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test window", nullptr, nullptr);

	std::cout << "Hello world!" << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	delete instance;

	return 0;
}