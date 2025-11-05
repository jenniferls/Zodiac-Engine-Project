#include "Zodiacpch.h"
#include "VulkanShaderModule.h"
#include "Initializers.h"
#include "Validation.h"

Zodiac::VulkanShaderModule::VulkanShaderModule(VulkanDevice* device, const char* path) {
	m_device = device->GetDevice();

	std::vector<char> content = ReadFile(path);
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

std::vector<char> Zodiac::VulkanShaderModule::ReadFile(const char* source) {
	std::ifstream file(source, std::ios::binary);
	if (!file.fail()) {
		std::streampos begin, end;
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();

		std::vector<char> result(static_cast<size_t>(end - begin));
		file.seekg(0, std::ios::beg);
		file.read(&result[0], end - begin);
		file.close();
		return result;
	}

	std::cout << "Could not open \"" << source << "\" file!" << std::endl;
	return std::vector<char>();
}
