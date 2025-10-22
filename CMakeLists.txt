cmake_minimum_required(VERSION 3.20)
Project(ZodiacEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ZodiacEngine)
add_subdirectory(ZodiacApplication)

foreach(util_target IN ITEMS ALL_BUILD INSTALL PACKAGE RUN_TESTS)
	if(TARGET ${util_target})
		set_target_properties(${util_target} PROPERTIES FOLDER "CMakeGeneratedTargets")
	endif()
endforeach()