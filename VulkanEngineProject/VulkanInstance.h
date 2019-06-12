#pragma once
#include <vector>

#include "Initializers.h"
#include "VulkanConfiguration.h"

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