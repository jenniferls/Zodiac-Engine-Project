#pragma once
#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include "Camera.h"
#include "Scene.h"

namespace Zodiac {
	struct RenderContext {
		//VulkanDevice* device;
		//std::vector<VkCommandBuffer> m_drawCmdBuffers;
		Camera* camera;
		Scene* scene;
	};
}