#pragma once

namespace Zodiac {
	class Application {
	public:
		Application();
		virtual ~Application() = default;

		void Run();

	private:

	};

	// To be defined in CLIENT
	Application* CreateApplication();
}