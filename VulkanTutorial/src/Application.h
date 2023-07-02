#pragma once

#include "VulkanWindow.h"
#include "VulkanEnginePipeline.h"

namespace VulkanEngine {

	class Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();
	private:
		VulkanWindow vulkanWindow{WIDTH, HEIGHT, "Hello Vulkan!" };
		VulkanEnginePipeline vulkanEnginePipeline{ "src/Shaders/SimpleShader.vert.spv", "src/Shaders/SimpleShader.frag.spv" };
	};

}

