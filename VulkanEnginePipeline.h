#pragma once

#include "VulkanDevice.h"
#include "VulkanModal.h"

#include <string>
#include <vector>

namespace VulkanEngine {

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;

		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VulkanEnginePipeline
	{
	public:
		VulkanEnginePipeline(VulkanDevice& vulkanDevice, const std::string& vertexFilePath, const std::string& fragmentShaderFilePath,
			const PipelineConfigInfo& configInfo);

		~VulkanEnginePipeline();

		VulkanEnginePipeline(const VulkanEnginePipeline&) = delete;
		VulkanEnginePipeline& operator=(const VulkanEnginePipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath, const PipelineConfigInfo& configInfo);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VulkanDevice& m_VulkanDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}