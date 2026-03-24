project(ZodiacApplication LANGUAGES CXX)

file(GLOB_RECURSE APP_SOURCES
	"*.cpp"
	"*.h"
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Find vulkan package
find_package(Vulkan REQUIRED)

# Include the engine headers
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Zodiac/include
)

# Eventually build for mac as well
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Zodiac/lib/ZodiacEngine.lib
    )
endif()

# For now since the engine is build as an executable
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/include
		${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/glm
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/lib-vc2015)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Vulkan::Vulkan
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../ZodiacEngine/Dependencies/GLFW/lib-vc2015/glfw3.lib
    )
elseif(APPLE)
    set(GLFW_LIB ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-universal/libglfw.3.dylib)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Shaders"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/ZodiacEngine/Resources"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources"
)
