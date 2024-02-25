#include "KeyboardMovementController.h"

#include <limits>
#include <glm/gtc/constants.hpp>

namespace VulkanEngine {

	void KeyboardController::MoveInPlaneXZ(GLFWwindow* window, float dt, VulkanEngineGameObject& gameObject)
	{
		glm::vec3 rotate{0};
		if (glfwGetKey(window, keys.LookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, keys.LookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, keys.LookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, keys.LookDown) == GLFW_PRESS) rotate.x -= 1.0f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) 
			gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		 

		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
	
		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
		const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
		const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

		glm::vec3 moveDir{ 0.0f };
		if (glfwGetKey(window, keys.MoveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.MoveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.MoveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.MoveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if (glfwGetKey(window, keys.MoveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.MoveDown) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
			gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
	}
}