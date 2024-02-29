#pragma once

#include "VulkanDevice.h"
#include "VulkanBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace VulkanEngine {

	class VulkanModal
	{
	public:
		struct Vertex 
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{}; 

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const 
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void LoadModel(const std::string& filepath);
		};

		VulkanModal(VulkanDevice& device, const Builder& builder);
		~VulkanModal();

		static std::unique_ptr<VulkanModal> CreateModelFromFile(VulkanDevice& device, const std::string& filepath);

		VulkanModal(const VulkanModal&) = delete;
		VulkanModal& operator=(const VulkanModal&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		VulkanDevice& vulkanDevice;

		std::unique_ptr<VulkanEngineBuffer> vertexBuffer;
		uint32_t vertexCount;

		std::unique_ptr<VulkanEngineBuffer> indexBuffer;
		bool hasIndexBuffer;
		uint32_t indexCount;
	};
}