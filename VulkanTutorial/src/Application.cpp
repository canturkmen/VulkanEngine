#include "Application.h"

#include <stdexcept>
#include <array>

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
			drawFrame();
		}

		vkDeviceWaitIdle(vulkanDevice.device());
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
		vulkanEnginePipeline = std::make_unique<VulkanEnginePipeline>(vulkanDevice, "src/Shaders/SimpleShader.vert.spv", 
			"src/Shaders/SimpleShader.frag.spv", pipelineConfig);
	}

	void Application::createCommandBuffers()
	{
		vulkanEngineCommandBuffers.resize(vulkanSwapChain.imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vulkanDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(vulkanEngineCommandBuffers.size());

		if (vkAllocateCommandBuffers(vulkanDevice.device(), &allocInfo, vulkanEngineCommandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers");

		for (int i = 0; i < vulkanEngineCommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			
			if (vkBeginCommandBuffer(vulkanEngineCommandBuffers[i], &beginInfo) != VK_SUCCESS)
				throw std::runtime_error("Failed to begin recording");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vulkanSwapChain.getRenderPass();
			renderPassInfo.framebuffer = vulkanSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vulkanSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(vulkanEngineCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vulkanEnginePipeline->Bind(vulkanEngineCommandBuffers[i]);
			vkCmdDraw(vulkanEngineCommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(vulkanEngineCommandBuffers[i]);
			if (vkEndCommandBuffer(vulkanEngineCommandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void Application::drawFrame()
	{
		uint32_t imageIndex;
		auto result = vulkanSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to require the next swap chain image");
		
		result = vulkanSwapChain.submitCommandBuffers(&vulkanEngineCommandBuffers[imageIndex], &imageIndex);
		if(result != VK_SUCCESS) 
			throw std::runtime_error("Failed to require the next swap chain image");
	}
}
