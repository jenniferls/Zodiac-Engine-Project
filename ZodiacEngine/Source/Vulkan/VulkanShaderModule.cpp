#include "Zodiacpch.h"
#include "VulkanShaderModule.h"
#include "Initializers.h"
#include "Validation.h"
#include "Utility.h"

Zodiac::VulkanShaderModule::VulkanShaderModule(VulkanDevice* device, const char* path) {
	m_device = device->GetDevice();

	std::vector<char> content = FileUtil::ReadBinaryFile(path);
	if (content.size() != 0) {
		VkShaderModuleCreateInfo shader_module_create_info = Initializers::ShaderModuleCreateInfo(content);
		ErrorCheck(vkCreateShaderModule(*m_device, &shader_module_create_info, nullptr, &m_shaderModule));
	}
	else {
		std::cout << "Error: Shader file is empty." << std::endl;
	}
}

Zodiac::VulkanShaderModule::VulkanShaderModule(VulkanDevice* device, std::vector<char> source)
{
	m_device = device->GetDevice();

	if (source.size() != 0) {
		VkShaderModuleCreateInfo shader_module_create_info = Initializers::ShaderModuleCreateInfo(source);
		ErrorCheck(vkCreateShaderModule(*m_device, &shader_module_create_info, nullptr, &m_shaderModule));
	}
	else {
		std::cout << "Error: Shader file is empty." << std::endl;
	}
}

Zodiac::VulkanShaderModule::~VulkanShaderModule() {
	vkDestroyShaderModule(*m_device, m_shaderModule, nullptr);
}

VkShaderModule* Zodiac::VulkanShaderModule::GetShaderModule() {
	return &m_shaderModule;
}
