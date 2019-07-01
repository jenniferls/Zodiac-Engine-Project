#include "Application.h"

Zodiac::Application* Zodiac::Application::s_instance = nullptr;

Zodiac::Application::Application() {
	s_instance = this;
	m_window = std::unique_ptr<Window>(Window::Create());
}

void Zodiac::Application::Init() {

}

void Zodiac::Application::Run() {
	while (m_running) {

	}
}

bool Zodiac::Application::OnWindowClose() {
	m_running = false;
	return true;
}
