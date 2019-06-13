#include <iostream>

#include "System.h"

Zodiac::System sys("Sandbox");

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (sys.init()) {
		std::cout << "Hello world!" << std::endl;

		sys.run();
	}

	system("pause");
	return 0;
}