#pragma once

#include "VulkanWindow.h"
#include "VulkanEnginePipeline.h"
#include "VulkanDevice.h"

namespace VulkanEngine {

	class Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();
	private:
		VulkanWindow vulkanWindow{WIDTH, HEIGHT, "Hello Vulkan!" };
		VulkanDevice vulkanDevice{vulkanWindow};
		VulkanEnginePipeline vulkanEnginePipeline{
			vulkanDevice, 
			"src/Shaders/SimpleShader.vert.spv", 
			"src/Shaders/SimpleShader.frag.spv", 
			VulkanEnginePipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};

}

