#include "Zodiac/Zodiac.h"

class Sandbox : public Zodiac::Application {
public:
	Sandbox();
	~Sandbox();

private:

};

Sandbox::Sandbox() {

}

Sandbox::~Sandbox() {

}

Zodiac::Application* Zodiac::CreateApplication() {
	return new Sandbox();
}