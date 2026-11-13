#pragma once
#include "Physics/Collider.h"

namespace TealEngine 
{
	class SphereCollider : public Collider
	{
	public:
		float radius;
		ColliderType getColliderType() 
		{
			return Collider::ColliderType::SPHERE;
		}

		virtual void getAABoundingBox(glm::vec3& from, glm::vec3& to);

		SphereCollider() 
		{
			radius = 1.0f;
			this->addProp(new FloatProp(&radius), "Radius");
		}
	};
}