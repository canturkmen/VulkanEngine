#include "Application.h"
#include "VulkanCamera.h"
#include "VulkanEngineRendererSystem.h"
#include "VulkanBuffer.h"

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

	struct GlobalUbo
	{
		glm::mat4 projectionView{ 1.0f };
		glm::vec3 lightDirection{ glm::normalize(glm::vec3{1.f, -3.f, -1.f})};
	};

	Application::Application() 
	{ 
		loadGameObjects(); 
	}

	Application::~Application() 
	{

	}

	void Application::Run()
	{
		VulkanEngineBuffer globalUboBuffer{
			vulkanDevice,
			sizeof(GlobalUbo),
			VulkanSwapChain::MAX_FRAMES_IN_FLIGHT,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			vulkanDevice.properties.limits.minUniformBufferOffsetAlignment,
		};

		globalUboBuffer.map();
			
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
				int frameIndex = vulkanRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};

				// update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				globalUboBuffer.writeToIndex(&ubo, frameIndex);
				globalUboBuffer.flushIndex(frameIndex);

				// render
				vulkanRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
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
		gameObject.transform.translation = { -.5f, .5f, 2.5f };
		gameObject.transform.scale = glm::vec3{ 3.0f, 1.5f, 3.0f };
		gameObjects.push_back(std::move(gameObject));

		std::shared_ptr<VulkanModal> smoothVaseModal = VulkanModal::CreateModelFromFile(vulkanDevice, "src/Models/smooth_vase.obj");
		auto smoothVase = VulkanEngineGameObject::CreateGameObject();
		smoothVase.modal = smoothVaseModal;
		smoothVase.transform.translation = { .5f, .5f, 2.5f };
		smoothVase.transform.scale = glm::vec3{ 3.0f, 1.5f, 3.0f };
		gameObjects.push_back(std::move(smoothVase));
	}
}