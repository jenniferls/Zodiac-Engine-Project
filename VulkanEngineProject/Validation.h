#pragma once
#include <iostream>
#include <assert.h>

#include <vulkan/vulkan.h>

namespace Zodiac {
	void ErrorCheck(VkResult result);

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
 }