#include "Zodiacpch.h"
#include "ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "Defines.h"
#include "Validation.h"
#include "Renderer.h"
#include "Initializers.h"

Zodiac::VulkanDevice* Zodiac::ImGuiLayer::s_device;
VkRenderPass Zodiac::ImGuiLayer::s_renderPass;
VkDescriptorPool Zodiac::ImGuiLayer::s_descriptorPool;


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

}

Zodiac::ImGuiLayer* Zodiac::ImGuiLayer::Create() {
	return new ImGuiLayer();
}

void Zodiac::ImGuiLayer::Shutdown()
{
	vkDestroyDescriptorPool(*s_device->GetDevice(), s_descriptorPool, nullptr);
	vkDestroyRenderPass(*s_device->GetDevice(), s_renderPass, nullptr);
	//ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_Shutdown();
	ImGui::DestroyContext();
}

bool Zodiac::ImGuiLayer::Init(GLFWwindow* window, VulkanDevice* device, VulkanInstance* instance) {
	s_device = device;

	ImGui_ImplGlfw_InitForVulkan(window, true);

	SetupDescriptorPool();
	SetupRenderPass();

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance->GetInstance();
	init_info.PhysicalDevice = s_device->GetPhysicalDevice()->GetPhysicalDevice();
	init_info.Device = *s_device->GetDevice();
	init_info.QueueFamily = s_device->GetPhysicalDevice()->GetFamilyIndices().graphics_indices;
	init_info.Queue = *s_device->GetGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE; //Should work for now
	init_info.DescriptorPool = s_descriptorPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = m_minImageCount;
	init_info.ImageCount = Zodiac::Renderer::s_swapchain->GetImageCount();
	init_info.CheckVkResultFn = ErrorCheck;
	ImGui_ImplVulkan_Init(&init_info, s_renderPass);

	return true;
}

void Zodiac::ImGuiLayer::SetupRenderPass()
{
	// Color attachment
	std::vector<VkAttachmentDescription> attachments(1);
	attachments[0].format = Zodiac::Renderer::s_swapchain->GetSurfaceFormat().format;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Setup attachment references
	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;													// Attachment 0 is color
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;				// Attachment layout used as color during the subpass

	// Setup a single subpass reference
	std::vector<VkSubpassDescription> subpassDescriptions(1);
	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].colorAttachmentCount = 1;									// Subpass uses one color attachment
	subpassDescriptions[0].pColorAttachments = &colorReference;							// Reference to the color attachment in slot 0

	std::vector<VkSubpassDependency> dependencies(1);
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = 0;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// Create the actual renderpass
	VkRenderPassCreateInfo renderPassInfo = Initializers::RenderPassCreateInfo(attachments, subpassDescriptions, dependencies);
	ErrorCheck(vkCreateRenderPass(*s_device->GetDevice(), &renderPassInfo, nullptr, &s_renderPass));
}

void Zodiac::ImGuiLayer::SetupDescriptorPool()
{
	//Allocating space for quite a lot of stuff, but we could still theoretically run out (although we shouldn't!)
	int32_t descriptorCount = 1000;
	VkDescriptorPoolSize poolSizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descriptorCount },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descriptorCount }
	};

	VkDescriptorPoolCreateInfo descriptorPoolInfo = Initializers::DescriptorPoolCreateInfo(IM_ARRAYSIZE(poolSizes), poolSizes, descriptorCount * IM_ARRAYSIZE(poolSizes), VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
	ErrorCheck(vkCreateDescriptorPool(*s_device->GetDevice(), &descriptorPoolInfo, nullptr, &s_descriptorPool));
}
