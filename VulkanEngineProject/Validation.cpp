#include "Zodiacpch.h"
#include "Validation.h"
//#include <iostream>

void Zodiac::ErrorCheck(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cout << "Error!" << std::endl;
		assert(0 && "An error has occured.");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL Zodiac::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
