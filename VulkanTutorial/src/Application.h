#pragma once

#include "VulkanWindow.h"
#include "VulkanEnginePipeline.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanEngineGameObject.h"

#include <memory>
#include <vector>

namespace VulkanEngine {

	class Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void Run();
	private:
		void loadGameObjects();
		void createPipeline();
		void createPipelineLayout();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		VulkanWindow vulkanWindow{WIDTH, HEIGHT, "Hello Vulkan!" };
		VulkanDevice vulkanDevice{vulkanWindow};
		std::unique_ptr<VulkanSwapChain> vulkanSwapChain;
		std::unique_ptr<VulkanEnginePipeline> vulkanEnginePipeline;
		VkPipelineLayout vulkanEnginePipelineLayout;
		std::vector<VkCommandBuffer> vulkanEngineCommandBuffers;
		std::vector<VulkanEngineGameObject> vulkanGameObjects;
	};

}

