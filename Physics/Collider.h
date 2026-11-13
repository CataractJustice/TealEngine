#pragma once
#include "GameNode/Component.h"

namespace TealEngine 
{
	class Collider : public Component
	{
	public:
		unsigned long long collidableByMask;
		unsigned long long collidesWithMask;
		bool removedFromScene;
		Collider(); 
		enum class ColliderType
		{
			ANY,
			AABC,
			AAPLANE,
			BOX,
			HEIGHTMAP,
			PLANE,
			SPHERE,
			SCALAR_FIELD,
			TILES
		};
		virtual ColliderType getColliderType() = 0;
		virtual void getAABoundingBox(glm::vec3& from, glm::vec3& to) { from = glm::vec3(0.0f); to = glm::vec3(0.0f); };
		inline void setRemoveFromScene(bool removed) { removedFromScene = removed; }
		inline bool isRemovedFromScene() { return removedFromScene; }
	};
}

