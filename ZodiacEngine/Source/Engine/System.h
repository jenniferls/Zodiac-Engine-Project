#pragma once
#include "Settings.h"
#include "Window.h"
#include "Clock.h"

#include "VulkanConfiguration.h"
#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanSurface.h"

#include "RenderContext.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputHandler.h"
#include "FileWatcher.h"
#include "Scene.h"

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
		Clock m_clock;
		FileWatcher m_fileWatcher;
		Camera* m_mainCamera;
		Scene m_scene;
		RenderContext m_renderContext;

		VulkanConfiguration m_vulkanConfig;
		VulkanInstance* m_instance;
		VulkanPhysicalDevice* m_physical_device;
		VulkanDevice* m_device;
		VulkanBuffer* m_buffer;
		VulkanSurface* m_surface;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<InputHandler> m_inputHandler;
	};
}