#pragma once
#include "Physics/Collider.h"

namespace TealEngine
{
	class ScalarFieldCollider : public Collider 
	{
		public:
		Collider::ColliderType getColliderType()
		{
			return Collider::ColliderType::SCALAR_FIELD;
		}

		inline virtual glm::vec3 getGridSize() {
			return glm::vec3(1.0f);
		}
		virtual float getScalarFieldAt(glm::vec3 position) = 0;
		virtual glm::vec3 getNormalAt(glm::vec3 position) = 0;
	};
} // namespace TealEngine
