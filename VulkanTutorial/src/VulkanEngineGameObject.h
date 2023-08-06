#pragma once 

#include "VulkanModal.h"

#include <memory>

namespace VulkanEngine {

	struct Transform2DComponent {
		glm::vec2 translation{}; // position
		glm::vec2 scale {1.0f, 1.0f}; // position
		float rotation;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c ,s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotMatrix * scaleMat;
		}
	};

	class VulkanEngineGameObject
	{
	public:
		using id_t = unsigned int; // id of our game objects

		VulkanEngineGameObject(const VulkanEngineGameObject&) = delete;
		VulkanEngineGameObject& operator=(const VulkanEngineGameObject&) = delete;
		VulkanEngineGameObject(VulkanEngineGameObject&&) = default;
		VulkanEngineGameObject& operator=(VulkanEngineGameObject&&) = default;
		
		static	VulkanEngineGameObject CreateGameObject() {
			static id_t currentId = 0;
			return VulkanEngineGameObject{ currentId++ };
		}

		id_t getId() { return id; }

		std::shared_ptr<VulkanModal> modal{};
		glm::vec3 color{};
		Transform2DComponent transform2DComponent;

	private:
		VulkanEngineGameObject(id_t objId) : id{ objId } {

		}

		id_t id;
	};
}


