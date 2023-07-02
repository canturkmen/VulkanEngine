#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace VulkanEngine {

	class VulkanWindow 
	{
	public:
		VulkanWindow(int w, int h, std::string name);
		~VulkanWindow();

		VulkanWindow(const VulkanWindow&) = delete;
		VulkanWindow& operator=(const VulkanWindow&) = delete; 

		bool ShouldClose() { return glfwWindowShouldClose(window); }

	private:
		void InitWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}

