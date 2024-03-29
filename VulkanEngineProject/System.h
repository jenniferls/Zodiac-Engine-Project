#pragma once
#include "Settings.h"
#include "ImGuiLayer.h"
#include "Window.h"

#include "VulkanConfiguration.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanSurface.h"

#include "Renderer.h"

//This class acts more or less as a testbed for now
namespace Zodiac {
	class System {
	public:
		System(const char* applicationName);
		~System();

		bool Init();
		void Run();

		void SetVSync(bool vsync);
		void SetFullscreen(bool fullscreen);

	private:
		bool InitVulkan();

		Settings m_settings;

		VulkanConfiguration m_vulkanConfig;
		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VulkanDevice* m_device;
		VulkanBuffer* m_buffer;
		VulkanSurface* m_surface;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<ImGuiLayer> m_imgui;
	};
}