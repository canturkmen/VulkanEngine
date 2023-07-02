#pragma once

#include "VulkanDevice.h"

#include <string>
#include <vector>

namespace VulkanEngine {

	struct PipelineConfigInfo
	{

	};
	
	class VulkanEnginePipeline 
	{
	public:
		VulkanEnginePipeline(VulkanDevice& vulkanDevice, const std::string& vertexFilePath, const std::string& fragmentShaderFilePath,
			const PipelineConfigInfo& configInfo);

		~VulkanEnginePipeline() {}

		VulkanEnginePipeline(const VulkanEnginePipeline&) = delete;
		void operator=(const VulkanEnginePipeline&) = delete;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

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
