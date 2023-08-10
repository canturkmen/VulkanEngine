#pragma once

#include "VulkanDevice.h"
#include "VulkanEngineGameObject.h"
#include "VulkanEngineRenderer.h"
#include "VulkanWindow.h"

// std
#include <memory>
#include <vector>

namespace VulkanEngine {

	class Application {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void run();

	private:
		void loadGameObjects();

		VulkanWindow vulkanWindow{ WIDTH, HEIGHT, "Vulkan Tutorial" };
		VulkanDevice vulkanDevice{ vulkanWindow };
		VulkanEngineRenderer vulkanRenderer{ vulkanWindow, vulkanDevice };

		std::vector<VulkanEngineGameObject> gameObjects;
	};
} 