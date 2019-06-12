#pragma once

struct VulkanConfiguration {
	const char* app_name = "";
	uint32_t app_version = VK_MAKE_VERSION(0, 0, 0);
	const char* engine_name = "Zodiac Engine";
	const uint32_t engine_version = VK_MAKE_VERSION(0, 0, 0);
	const uint32_t api_version = VK_MAKE_VERSION(1, 1, 106, 0);
};