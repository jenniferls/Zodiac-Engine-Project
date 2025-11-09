project(ZodiacEngine LANGUAGES CXX)

# Get any stray source files
file(GLOB_RECURSE ENGINE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# Automatically create Visual Studio filters based on folder + file type
foreach(FILE_PATH ${ENGINE_SOURCES})
	get_filename_component(FILE_EXT ${FILE_PATH} EXT)

	get_filename_component(PARENT_DIR ${FILE_PATH} DIRECTORY)
	file(RELATIVE_PATH PARENT_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR} ${PARENT_DIR})

	if(FILE_EXT STREQUAL ".h" OR FILE_EXT STREQUAL ".hpp")
		set(GROUP_NAME "Header Files")
	elseif(FILE_EXT STREQUAL ".cpp" OR FILE_EXT STREQUAL ".c")
		set(GROUP_NAME "Source Files")
	else()
		set(GROUP_NAME "Other")
	endif()

	set(FILTER_PATH "${PARENT_DIR_REL}\\${GROUP_NAME}")

	source_group("${FILTER_PATH}" FILES "${FILE_PATH}")
endforeach()

# Build as executable for now, change to library later
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES})

# Precompiled header
target_precompile_headers(ZodiacEngine
	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/Zodiacpch.h"
)

# Find vulkan package
find_package(Vulkan REQUIRED)

# Include directories
target_include_directories(${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/Source
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Vulkan
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Animation
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Assets
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Input
		${CMAKE_CURRENT_SOURCE_DIR}/Source/Sound
		${CMAKE_CURRENT_SOURCE_DIR}/Source/UI
		${CMAKE_CURRENT_SOURCE_DIR}/Source/ImGui
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glm
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/entt
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/SPIRV-Reflect
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/slang/include
		"$ENV{VULKAN_SDK}/Include"
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2015
		${Vulkan_INCLUDE_DIRS}
)

# Set GLFW path
set(GLFW_LIB ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2015/glfw3.lib)

target_link_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/slang/lib
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
		PRIVATE
			${Vulkan_LIBRARIES}
			${GLFW_LIB}
			slang.lib
)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui)
file(GLOB IMGUI_SOURCES
	${IMGUI_DIR}/*.cpp
)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC
	${IMGUI_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include
	"$ENV{VULKAN_SDK}/Include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)

# Post-build, copy shaders and resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Slang/bin/slang.dll"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Slang/bin/slang-compiler.dll"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Slang/bin/slangc.pdb"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Slang/bin/slang-compiler.pdb"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Shaders"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Resources"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources"
)