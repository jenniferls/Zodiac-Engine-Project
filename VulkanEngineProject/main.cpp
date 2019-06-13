#include <windows.h>
#include <iostream>

#include "System.h"

Zodiac::System sys("Sandbox");

int main(int argc, char **argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (sys.init()) {
		std::cout << "Hello world!" << std::endl;

		sys.run();
	}
	return 0;
}