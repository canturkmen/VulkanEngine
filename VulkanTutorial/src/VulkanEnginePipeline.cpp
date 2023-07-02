#include "VulkanEnginePipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace VulkanEngine {

	VulkanEnginePipeline::VulkanEnginePipeline(VulkanDevice& vulkanDevice, const std::string& vertexFilePath, const std::string& fragmentShaderFilePath,
		const PipelineConfigInfo& configInfo) : m_VulkanDevice(vulkanDevice)
	{
		createGraphicsPipeline(vertexFilePath, fragmentShaderFilePath, configInfo);
	}

	PipelineConfigInfo VulkanEnginePipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo;
		return configInfo;
	}

	std::vector<char> VulkanEnginePipeline::readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filePath);

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void VulkanEnginePipeline::createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath, const PipelineConfigInfo& configInfo)
	{
		const auto& vertexCode = readFile(vertexFilePath);
		const auto& fragCode = readFile(fragmentShaderFilePath);

		std::cout << "Vertex Shader Code Size: " << vertexCode.size() << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
	}

	void VulkanEnginePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(m_VulkanDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader modules");
	}

}