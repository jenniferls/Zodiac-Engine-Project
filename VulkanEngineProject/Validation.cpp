#include "Validation.h"

void Zodiac::ErrorCheck(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cout << "Error!" << std::endl;
		assert(0 && "An error has occured.");
	}
}
