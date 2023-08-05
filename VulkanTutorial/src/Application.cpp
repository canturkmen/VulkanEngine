#include "Application.h"

#include <stdexcept>
#include <array>

namespace VulkanEngine {

	Application::Application()
	{
		loadModals();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
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

	void Application::loadModals()
	{
		std::vector<VulkanModal::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		vulkanModal = std::make_unique<VulkanModal>(vulkanDevice, vertices);
	}

	void Application::createPipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanEnginePipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = vulkanSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = vulkanEnginePipelineLayout;
		vulkanEnginePipeline = std::make_unique<VulkanEnginePipeline>(vulkanDevice, "src/Shaders/SimpleShader.vert.spv", 
			"src/Shaders/SimpleShader.frag.spv", pipelineConfig);
	}

	void Application::recreateSwapChain()
	{
		auto extent = vulkanWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = vulkanWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(vulkanDevice.device());

		if(vulkanSwapChain == nullptr) 
			vulkanSwapChain = std::make_unique<VulkanSwapChain>(vulkanDevice, extent);
		else {
			vulkanSwapChain = std::make_unique<VulkanSwapChain>(vulkanDevice, extent, std::move(vulkanSwapChain));	
			if (vulkanSwapChain->imageCount() != vulkanEngineCommandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}

	void Application::createCommandBuffers()
	{
		vulkanEngineCommandBuffers.resize(vulkanSwapChain->imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vulkanDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(vulkanEngineCommandBuffers.size());

		if (vkAllocateCommandBuffers(vulkanDevice.device(), &allocInfo, vulkanEngineCommandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers");
	}

	void Application::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			vulkanDevice.device(),
			vulkanDevice.getCommandPool(),
			static_cast<float>(vulkanEngineCommandBuffers.size()),
			vulkanEngineCommandBuffers.data()
		);

		vulkanEngineCommandBuffers.clear();
	}

	void Application::drawFrame()
	{
		uint32_t imageIndex;
		auto result = vulkanSwapChain->acquireNextImage(&imageIndex);
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to require the next swap chain image");
		 
		recordCommandBuffer(imageIndex);
		result = vulkanSwapChain->submitCommandBuffers(&vulkanEngineCommandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanWindow.WasWindowResized())
		{
			vulkanWindow.ResetWindowResizedFlag();
			recreateSwapChain();
			return;
		}	

		if(result != VK_SUCCESS) 
			throw std::runtime_error("Failed to require the next swap chain image");
	}

	void Application::recordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(vulkanEngineCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin recording");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vulkanSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vulkanSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(vulkanEngineCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vulkanSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vulkanSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vulkanSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(vulkanEngineCommandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(vulkanEngineCommandBuffers[imageIndex], 0, 1, &scissor);


		vulkanEnginePipeline->Bind(vulkanEngineCommandBuffers[imageIndex]);
		vulkanModal->Bind(vulkanEngineCommandBuffers[imageIndex]);
		vulkanModal->Draw(vulkanEngineCommandBuffers[imageIndex]);

		vkCmdEndRenderPass(vulkanEngineCommandBuffers[imageIndex]);
		if (vkEndCommandBuffer(vulkanEngineCommandBuffers[imageIndex]) != VK_SUCCESS)
			throw std::runtime_error("Failed to record command buffer!");
	}

}
