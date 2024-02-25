#include "Application.h"
#include "VulkanCamera.h"
#include "VulkanEngineRendererSystem.h"

#include "KeyboardMovementController.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>
#include <chrono>

namespace VulkanEngine {

	Application::Application() 
	{ 
		loadGameObjects(); 
	}

	Application::~Application() 
	{

	}

	void Application::Run()
	{
		VulkanEngineRendererSystem simpleRenderSystem{ vulkanDevice, vulkanRenderer.getSwapChainRenderPass() };
		VulkanCamera camera{};

		camera.SetViewTarget({-1.f, -2.f, 2.f}, {0.0f, 0.0f, 2.5f});
		auto viewerObject = VulkanEngineGameObject::CreateGameObject();
		KeyboardController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vulkanWindow.ShouldClose()) {
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.MoveInPlaneXZ(vulkanWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = vulkanRenderer.getAspectRatio();
			camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			if (auto commandBuffer = vulkanRenderer.beginFrame()) 
			{
				vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				vulkanRenderer.endSwapChainRenderPass(commandBuffer);
				vulkanRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vulkanDevice.device());
	}

	void Application::loadGameObjects() 
	{
		std::shared_ptr<VulkanModal> vulkanModal = VulkanModal::CreateModelFromFile(vulkanDevice, "src/Models/flat_vase.obj");
		auto gameObject = VulkanEngineGameObject::CreateGameObject();
		gameObject.modal = vulkanModal;
		gameObject.transform.translation = { .0f, .0f, 2.5f };
		gameObject.transform.scale = glm::vec3{ 3.0f };
		gameObjects.push_back(std::move(gameObject));
	}
}