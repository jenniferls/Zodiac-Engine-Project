#include "System.h"
//#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) //For detecting memory leaks

Zodiac::System::System(const char* applicationName) {
	m_vulkanConfig.app_name = applicationName;
	m_vulkanConfig.app_version = VK_MAKE_VERSION(0, 1, 0);
}

Zodiac::System::~System() {
	delete m_buffer;
	delete m_device;
	delete m_physical_device;
	delete m_instance;
}

bool Zodiac::System::init() {
	m_instance = new Zodiac::VulkanInstance(m_vulkanConfig);
	m_physical_device = Zodiac::VulkanPhysicalDevice::GetPhysicalDevice(m_instance);
	m_device = new Zodiac::VulkanDevice(m_instance, m_physical_device);

	/////// Tests ///////
	VkCommandBuffer* commands = new VkCommandBuffer[3]; //Command buffers test
	m_device->GetComputeCommand(commands, 3);

	float* arr = new float[3];
	for (int i = 0; i < 3; i++) { //Test
		arr[i] = (float)i;
	}

	m_buffer = new Zodiac::VulkanBuffer(m_device, arr, sizeof(float), 3);
	m_buffer->SetData();

	m_device->FreeComputeCommand(commands, 3);
	delete commands;
	delete[] arr;
	//////////////////

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(800, 600, m_vulkanConfig.app_name, nullptr, nullptr);

	return true;
}

void Zodiac::System::run() {
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
	}
	shutdown();
}

void Zodiac::System::shutdown() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
