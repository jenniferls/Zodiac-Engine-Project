#pragma once
#include "BUILD_ORDER.h"

class VulkanInstance {
public:
	VulkanInstance(VulkanConfiguration& config);
	~VulkanInstance();

	VkInstance& GetInstance();

private:
	VkInstance instance;
	std::vector<const char*> layers;
	std::vector<const char*> extensions;
};