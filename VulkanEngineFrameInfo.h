#pragma once

#include "VulkanCamera.h"

#include <vulkan/vulkan.h>

namespace VulkanEngine {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VulkanCamera& camera;
	};
}
