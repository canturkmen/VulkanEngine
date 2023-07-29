#include "Application.h"

#include <stdexcept>

namespace VulkanEngine {

	Application::Application()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
		drawFrame();
	}

	Application::~Application()
	{
		vkDestroyPipelineLayout(vulkanDevice.device(), vulkanEnginePipelineLayout, nullptr);
	}

	void Application::Run()
	{
		while (!vulkanWindow.ShouldClose())
		{
			glfwPollEvents();
		}
	}

	void Application::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(vulkanDevice.device(), &pipelineLayoutInfo, nullptr, &vulkanEnginePipelineLayout) != VK_SUCCESS) 
			throw std::runtime_error("Failed to create pipeline layout");
	}

	void Application::createPipeline()
	{
		auto pipelineConfig = VulkanEnginePipeline::DefaultPipelineConfigInfo(vulkanSwapChain.width(), vulkanSwapChain.height());
		pipelineConfig.renderPass = vulkanSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = vulkanEnginePipelineLayout;
		vulkanEnginePipeline = std::make_unique<VulkanEnginePipeline>(vulkanDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void Application::createCommandBuffers()
	{

	}

	void Application::drawFrame()
	{

	}
}
