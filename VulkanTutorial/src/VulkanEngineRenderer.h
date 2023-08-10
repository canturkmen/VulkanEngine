#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanWindow.h"

#include <cassert>
#include <memory>
#include <vector>

namespace VulkanEngine {

	class VulkanEngineRenderer {
	public:
		VulkanEngineRenderer(VulkanWindow& window, VulkanDevice& device);
		~VulkanEngineRenderer();

		VulkanEngineRenderer(const VulkanEngineRenderer&) = delete;
		VulkanEngineRenderer& operator=(const VulkanEngineRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return vulkanSwapChain->getRenderPass(); }
		float getAspectRatio() const { return vulkanSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const 
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const 
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VulkanWindow& vulkanWindow;
		VulkanDevice& vulkanDevice;
		std::unique_ptr<VulkanSwapChain> vulkanSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;
		
		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};
}