#include "VulkanWindow.h"

namespace VulkanEngine {

	VulkanWindow::VulkanWindow(int w, int h, std::string name)
		: width{w}, height{h}, windowName{name}
	{
		InitWindow();
	}
	
	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	
	void VulkanWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}

