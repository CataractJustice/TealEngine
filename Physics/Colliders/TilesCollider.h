#pragma once
#include "Physics/Collider.h"

namespace TealEngine
{
	class TilesCollider : public Collider 
	{
		public:
		Collider::ColliderType getColliderType()
		{
			return Collider::ColliderType::TILES;
		}
		
		virtual std::vector<Collider*> getCollidersAt(glm::vec3 position) = 0;
	};
} // namespace TealEngine
