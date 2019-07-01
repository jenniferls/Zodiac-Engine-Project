#pragma once
#include <memory>
#include "Window.h"

namespace Zodiac {
	class Application {
	public:
		Application();
		virtual ~Application() = default; //Instead of empty destructor

		void Init();
		void Run();

	private:
		bool OnWindowClose();

		static Application* s_instance;
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}