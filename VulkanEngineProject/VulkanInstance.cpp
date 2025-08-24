#include "Zodiacpch.h"
#include "VulkanInstance.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanInstance::VulkanInstance(VulkanConfiguration& config, const char** glfwExtensions, int glfwExtCount) {
	m_layers.push_back("VK_LAYER_KHRONOS_validation");
	//m_layers.push_back("VK_LAYER_RENDERDOC_Capture");
	//m_layers.push_back("VK_LAYER_LUNARG_assistant_layer");
	//m_layers.push_back("VK_LAYER_LUNARG_api_dump"); //Detailed information about API calls, their parameters and values
	m_layers.push_back("VK_LAYER_LUNARG_monitor"); //For printing fps to window title bar
	m_extensions.push_back("VK_EXT_debug_utils");
	//m_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME); //Already in glfwExtensions

	for (int i = 0; i < glfwExtCount; i++) {
		m_extensions.push_back(glfwExtensions[i]);
	}

	UpdateInstanceVersion();
	config.api_version = GetInstanceVersion();
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

uint32_t Zodiac::VulkanInstance::GetInstanceVersion() {
	return VK_MAKE_API_VERSION(0, m_instanceVersion.Major, m_instanceVersion.Minor, m_instanceVersion.Patch);
}

const int Zodiac::VulkanInstance::GetMajorInstanceVersion()
{
	return m_instanceVersion.Major;
}

const int Zodiac::VulkanInstance::GetMinorInstanceVersion()
{
	return m_instanceVersion.Minor;
}

bool Zodiac::VulkanInstance::IsInstanceVersionOrAbove(uint32_t major, uint32_t minor)
{
	return (m_instanceVersion.Major > major) || (m_instanceVersion.Minor >= minor);
}

void Zodiac::VulkanInstance::UpdateInstanceVersion() {
	uint32_t InstanceVersion = 0;
	ErrorCheck(vkEnumerateInstanceVersion(&InstanceVersion));
	m_instanceVersion.Major = VK_API_VERSION_MAJOR(InstanceVersion);
	m_instanceVersion.Minor = VK_API_VERSION_MINOR(InstanceVersion);
	m_instanceVersion.Patch = VK_API_VERSION_PATCH(InstanceVersion);

	printf("Vulkan loader supports version %d.%d.%d\n", m_instanceVersion.Major, m_instanceVersion.Minor, m_instanceVersion.Patch);
}
