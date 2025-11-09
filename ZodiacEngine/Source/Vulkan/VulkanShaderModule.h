#pragma once
#include <vulkan\vulkan.h>
#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanShaderModule {
	public:
		VulkanShaderModule(VulkanDevice* device, const char* path);
		VulkanShaderModule(VulkanDevice* device, std::vector<char> source); //Create from precompiled shader data directly
		VulkanShaderModule(VulkanDevice* device, const uint32_t* spirv, size_t size);
		~VulkanShaderModule();

		VkShaderModule* GetShaderModule();

	private:
		VkShaderModule m_shaderModule = nullptr;
		VkDevice* m_device;
	};
}