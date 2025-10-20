#pragma once
#include <vulkan\vulkan.h>
#include "VulkanDevice.h"

namespace Zodiac {
	class VulkanShaderModule {
	public:
		VulkanShaderModule(VulkanDevice* device, const char* source);
		~VulkanShaderModule();

		VkShaderModule* GetShaderModule();

	private:
		std::vector<char> ReadFile(const char* source);

		VkShaderModule m_shaderModule = nullptr;
		VkDevice* m_device;
	};
}