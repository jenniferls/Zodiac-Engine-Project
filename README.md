# Zodiac-Engine-Project
A Vulkan engine project to teach myself about the Vulkan API and modern rendering techniques. The main goal of the project is to explore SPU-driven rendering and create a sandbox environment for learning about and implementing tech used by modern real-time engines. The project was started using Vulkan 1.1 but have since then been upgraded to support newer vulkan features.

## CMake version
Requires CMake 3.20 minimum

## Vulkan usage
Vulkan version: 1.4

Uses dynamic rendering for imgui but render passes for the main render.

## Third party libraries
- glfw
- glm
- ImGui
- EnTT
- slang
- assimp

## Implemented features
- Shader hot-reloading
- Programmable Vertex Pulling with Indexed rendering
- Dynamic rendering + classic render passes
- VSync
- Toggleable imgui layer
- Mouse and keyboard-controlled camera
