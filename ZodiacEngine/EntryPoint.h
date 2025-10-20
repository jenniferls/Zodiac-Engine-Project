#pragma once

extern Zodiac::Application* Zodiac::CreateApplication();

int main(int argc, char** argv) {
	auto app = Zodiac::CreateApplication();
	app->Run();
	delete app;
}