//This file is used for testing the library by building as an .exe

#include "Zodiacpch.h"
//#include <iostream>

#include "System.h"

Zodiac::System sys("Sandbox");

int main(int argc, char **argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Debug memory leaks
	std::cout << ">>>> Initializing Zodiac Engine <<<<" << std::endl;
	if (sys.Init()) {
		std::cout << "Zodiac Initialization successful!" << std::endl;
		sys.Run();
	}
	return 0;
}