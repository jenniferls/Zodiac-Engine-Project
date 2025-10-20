#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace Zodiac {
	struct SurfaceDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> supported_formats;
		std::vector<VkPresentModeKHR> supported_present_modes;
	};
}