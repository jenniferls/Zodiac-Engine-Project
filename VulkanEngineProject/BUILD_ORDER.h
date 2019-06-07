#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <assert.h>

#include <vulkan/vulkan.h>

#include "Validation.h"
#include "VulkanConfiguration.h"
#include "Initializers.h"
#include "VulkanInstance.h"