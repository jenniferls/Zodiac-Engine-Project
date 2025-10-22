cmake_minimum_required(VERSION 3.20)
Project(ZodiacEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ZodiacEngine)

add_subdirectory(ZodiacEngine)
add_subdirectory(ZodiacApplication)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		SHADERS_DIR="$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
)

foreach(util_target IN ITEMS ALL_BUILD INSTALL PACKAGE RUN_TESTS)
	if(TARGET ${util_target})
		set_target_properties(${util_target} PROPERTIES FOLDER "CMakeGeneratedTargets")
	endif()
endforeach()