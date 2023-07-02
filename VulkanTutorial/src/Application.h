#pragma once

#include "VulkanWindow.h"

namespace VulkanEngine {

	class Application
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();
	private:
		VulkanWindow vulkanWindow{WIDTH, HEIGHT, "Hello Vulkan!" };
	};

}

