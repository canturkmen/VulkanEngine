#pragma once

#include "VulkanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace VulkanEngine {

	class VulkanModal
	{
	public:
		struct Vertex 
		{
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		VulkanModal(VulkanDevice& device, const std::vector<Vertex>& vertices);
		~VulkanModal();

		VulkanModal(const VulkanModal&) = delete;
		VulkanModal& operator=(const VulkanModal&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		VulkanDevice& vulkanDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};

}


