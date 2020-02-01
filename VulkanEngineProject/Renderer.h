#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include "vulkan/vulkan.h"
//#include "VulkanInstance.h"
//#include "VulkanPhysicalDevice.h"
//#include "VulkanDevice.h"
//#include "VulkanSwapchain.h"

namespace Zodiac {
	class Renderer {
	public:
		Renderer(/*VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanDevice* device*/);
		~Renderer();

		static void BeginScene();
		static void Submit();
		static void EndScene();
		static void Flush();

		//void SetClearColor(const glm::vec4 color);
		void Clear();

		void DrawIndexed();

	private:
		//VulkanInstance* m_instance;
		//VulkanPhysicalDevice* m_physicalDevice;
		//VulkanDevice* m_device;

		//VulkanSwapchain m_swapchain;

	};
}