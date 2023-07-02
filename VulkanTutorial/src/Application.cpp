#include "Application.h"

namespace VulkanEngine {

	void Application::Run() 
	{
		while (!vulkanWindow.ShouldClose()) 
		{
			glfwPollEvents();
		}
	}
}
