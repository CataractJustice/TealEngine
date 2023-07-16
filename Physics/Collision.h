#pragma once
#include "libs/glm/glm.hpp"

namespace TealEngine
{
	class Collider;

	struct Collision 
	{
		Collider* collider;
		Collider* collidedWith;
		glm::vec3 pos;
		glm::vec3 relativePos;
		glm::vec3 normal;
		float depth;
	};
} // namespace TealEngine
