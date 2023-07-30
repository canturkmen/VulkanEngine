#pragma once

#include "VulkanWindow.h"
#include "VulkanEnginePipeline.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

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
		void loadModals();
		void createPipeline();
		void createPipelineLayout();
		void createCommandBuffers();
		void drawFrame();

		VulkanWindow vulkanWindow{WIDTH, HEIGHT, "Hello Vulkan!" };
		VulkanDevice vulkanDevice{vulkanWindow};
		VulkanSwapChain vulkanSwapChain{vulkanDevice, vulkanWindow.getExtent()};
		std::unique_ptr<VulkanEnginePipeline> vulkanEnginePipeline;
		VkPipelineLayout vulkanEnginePipelineLayout;
		std::vector<VkCommandBuffer> vulkanEngineCommandBuffers;
		std::unique_ptr<VulkanModal> vulkanModal;
	};

}

