cmake_minimum_required(VERSION 3.20)
Project(ZodiacEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ZodiacEngine)
endif()

add_subdirectory(ZodiacEngine)
# Haven't built the engine library on Mac yet so for now, don't build the application
if (NOT APPLE)
    add_subdirectory(ZodiacApplication)
endif()

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		$<$<CONFIG:Debug>:SHADERS_DIR="${CMAKE_SOURCE_DIR}/ZodiacEngine/Shaders">
		$<$<NOT:$<CONFIG:Debug>>:SHADERS_DIR="$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders">
		IMPORT_MODELS_DIR="${CMAKE_SOURCE_DIR}/ZodiacEngine/Resources/Models/Import"
)

foreach(util_target IN ITEMS ALL_BUILD INSTALL PACKAGE RUN_TESTS)
	if(TARGET ${util_target})
		set_target_properties(${util_target} PROPERTIES FOLDER "CMakeGeneratedTargets")
	endif()
endforeach()
