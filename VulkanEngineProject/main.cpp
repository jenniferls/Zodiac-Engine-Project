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

Zodiac::VulkanInstance* g_instance;
Zodiac::VulkanPhysicalDevice* g_physical_device;
Zodiac::VulkanDevice* g_device;
Zodiac::VulkanBuffer* g_buffer;

int main() {
	Zodiac::VulkanConfiguration vulkan_config;
	vulkan_config.app_name = "Sandbox";
	vulkan_config.app_version = VK_MAKE_VERSION(0, 1, 0);

	g_instance = new Zodiac::VulkanInstance(vulkan_config);
	g_physical_device = Zodiac::VulkanPhysicalDevice::GetPhysicalDevice(g_instance);
	g_device = new Zodiac::VulkanDevice(g_instance, g_physical_device);

	VkCommandBuffer* commands = new VkCommandBuffer[3]; //Command buffers test
	g_device->GetComputeCommand(commands, 3);

	float* arr = new float[3];
	for (int i = 0; i < 3; i++) { //Test
		arr[i] = i;
	}

	g_buffer = new Zodiac::VulkanBuffer(g_device, arr, sizeof(float), 3);
	g_buffer->SetData();

	g_device->FreeComputeCommand(commands, 3);

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

	delete g_buffer;
	delete arr;
	delete g_device;
	delete g_physical_device;
	delete g_instance;

	system("pause");
	return 0;
}