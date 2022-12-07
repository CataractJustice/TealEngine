#pragma once
#include "libs/glm/glm.hpp"
namespace TealEngine
{
	namespace Physics
	{
		class Collider;
		class Collision
		{
		public:
			Collision(Collider* collider, Collider* collidedWith, float depth, glm::vec3 position, glm::vec3 normal);
			Collider* collidedWith;
			Collider* collider;
			float depth;
			glm::vec3 position;
			glm::vec3 normal;
		};
	}
}