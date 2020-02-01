#pragma once
#include "VulkanConfiguration.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanSwapchain.h"
#include "VulkanSemaphore.h"

#include "Settings.h"
#include "Window.h"

//This class acts more or less as a testbed for now
namespace Zodiac {
	class System {
	public:
		System(const char* applicationName);
		~System();

		bool Init();
		void Run();

	private:
		bool InitVulkan();

		VulkanConfiguration m_vulkanConfig;
		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VulkanDevice* m_device;
		VulkanBuffer* m_buffer;
		VulkanSwapchain* m_swapchain;

		VulkanSemaphore* m_presentSemaphore;
		VulkanSemaphore* m_renderCompleteSemaphore;

		std::unique_ptr<Window> m_window;
	};
}