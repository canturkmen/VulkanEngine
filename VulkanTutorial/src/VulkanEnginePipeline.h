#pragma once

#include <string>
#include <vector>

namespace VulkanEngine {
	
	class VulkanEnginePipeline 
	{
	public:
		VulkanEnginePipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentShaderFilePath);
	};
}
