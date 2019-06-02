#pragma once
#include "BUILD_ORDER.h"

namespace Initializers {
	VkApplicationInfo ApplicationInfo(VulkanConfiguration& config);
	VkInstanceCreateInfo InstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& layers, std::vector<const char*>& extensions);
}