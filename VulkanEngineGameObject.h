#pragma once 

#include "VulkanModal.h"

#include <memory>

namespace VulkanEngine {

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class VulkanEngineGameObject
	{
	public:
		using id_t = unsigned int; // id of our game objects

		VulkanEngineGameObject(const VulkanEngineGameObject&) = delete;
		VulkanEngineGameObject& operator=(const VulkanEngineGameObject&) = delete;
		VulkanEngineGameObject(VulkanEngineGameObject&&) = default;
		VulkanEngineGameObject& operator=(VulkanEngineGameObject&&) = default;
		
		static	VulkanEngineGameObject CreateGameObject() 
		{
			static id_t currentId = 0;
			return VulkanEngineGameObject{ currentId++ };
		}

		id_t getId() { return id; }

		std::shared_ptr<VulkanModal> modal{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		VulkanEngineGameObject(id_t objId) : id{ objId } 
		{

		}

		id_t id;
	};
}


