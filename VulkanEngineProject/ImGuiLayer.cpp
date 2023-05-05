#include "Zodiacpch.h"
#include "ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "Defines.h"

Zodiac::ImGuiLayer::ImGuiLayer() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();

	std::cout << "ImGui context successfully created." << std::endl;
}

Zodiac::ImGuiLayer::~ImGuiLayer() {
	ImGui::DestroyContext();
}

Zodiac::ImGuiLayer* Zodiac::ImGuiLayer::Create() {
	return new ImGuiLayer();
}

bool Zodiac::ImGuiLayer::Init(GLFWwindow* window, VulkanDevice* device) {
	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	//init_info.Instance = g_Instance;
	//init_info.PhysicalDevice = device->GetPhysicalDevice()->GetPhysicalDevice();
	//init_info.Device = *device->GetDevice();
	//init_info.QueueFamily = g_QueueFamily;
	//init_info.Queue = g_Queue;
	//init_info.PipelineCache = g_PipelineCache;
	//init_info.DescriptorPool = m_descriptorPool;
	//init_info.Allocator = g_Allocator;
	//init_info.MinImageCount = g_MinImageCount;
	//init_info.ImageCount = wd->ImageCount;
	//init_info.CheckVkResultFn = check_vk_result;
	//ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

	return true;
}
