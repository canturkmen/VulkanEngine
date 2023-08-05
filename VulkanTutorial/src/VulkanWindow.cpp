#include "VulkanWindow.h"

#include <stdexcept>

namespace VulkanEngine {

	VulkanWindow::VulkanWindow(int w, int h, std::string name)
		: width{w}, height{h}, windowName{name}
	{
		InitWindow();
	}
	
	void VulkanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) 
			throw std::runtime_error("Failed to create window surface");
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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FrameBufferResizedCallback);
	}

	void VulkanWindow::FrameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto vulkanEngineWindow = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
		vulkanEngineWindow->frameBufferResized = true;
		vulkanEngineWindow->width = width;
		vulkanEngineWindow->height = height;
	}

}

