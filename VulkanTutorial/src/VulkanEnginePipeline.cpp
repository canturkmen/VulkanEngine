#include "VulkanEnginePipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace VulkanEngine {

	VulkanEnginePipeline::VulkanEnginePipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath)
	{
		createGraphicsPipeline(vertexFilePath, fragmentShaderFilePath);
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

	void VulkanEnginePipeline::createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath)
	{
		const auto& vertexCode = readFile(vertexFilePath);
		const auto& fragCode = readFile(fragmentShaderFilePath);

		std::cout << "Vertex Shader Code Size: " << vertexCode.size() << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
	}
}