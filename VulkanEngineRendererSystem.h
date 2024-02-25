#pragma once

#include "VulkanCamera.h"
#include "VulkanDevice.h"
#include "VulkanEngineGameObject.h"
#include "VulkanEnginePipeline.h"

#include <memory>
#include <vector>

namespace VulkanEngine {
	class VulkanEngineRendererSystem {
	public:
		VulkanEngineRendererSystem(VulkanDevice& device, VkRenderPass renderPass);
		~VulkanEngineRendererSystem();

		VulkanEngineRendererSystem(const VulkanEngineRendererSystem&) = delete;
		VulkanEngineRendererSystem& operator=(const VulkanEngineRendererSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VulkanEngineGameObject>& gameObjects, const VulkanCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VulkanDevice& vulkanDevice;

		std::unique_ptr<VulkanEnginePipeline> vulkanPipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve