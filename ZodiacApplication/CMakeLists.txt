project(ZodiacApplication LANGUAGES CXX)

file(GLOB_RECURSE APP_SOURCES
	"*.cpp"
	"*.h"
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Include the engine headers
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Zodiac/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Zodiac/lib/ZodiacEngine.lib
)

# For now since the engine is build as an executable
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/include
		${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/glm
		$ENV{VULKAN_SDK}/Include
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/lib-vc2015)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/lib-vc2015/glfw3.lib
		${Vulkan_LIBRARIES}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Shaders"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Resources"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources"
)