#pragma once

#include "VulkanEngineGameObject.h"
#include "VulkanWindow.h"

namespace VulkanEngine {

	class KeyboardController
	{
	public:

		struct KeyMappings
		{
			int MoveLeft = GLFW_KEY_A;
			int MoveRight = GLFW_KEY_D;
			int MoveForward = GLFW_KEY_W;
			int MoveBackward = GLFW_KEY_S;
			int MoveUp = GLFW_KEY_E;
			int MoveDown = GLFW_KEY_Q;
			int LookLeft = GLFW_KEY_LEFT;
			int LookRight = GLFW_KEY_RIGHT;
			int LookUp = GLFW_KEY_UP;
			int LookDown = GLFW_KEY_DOWN;
		};

		void MoveInPlaneXZ(GLFWwindow* window, float dt, VulkanEngineGameObject& gameObject);

		KeyMappings keys{};
		float moveSpeed{ 3.0f };
		float lookSpeed{ 1.5f };
	};
}