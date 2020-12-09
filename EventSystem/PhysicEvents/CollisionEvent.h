#pragma once
#include "../Event.h"
#include "../../GameNode/GameNode3D.h"
#include "../../Physics/Collider.h"
#include "../../Physics/RigidBody.h"
namespace TealEngine
{
	class CollisionEvent : public Event
	{
	public:
		//pointer to collided collider
		//Collider *const collider;
		//rigidbody to witch collider are related, if is a static collider then equals to null
		//RigidBody *const body;
		//just a normal
		vec3 normal;
		//center of intersection in world coords
		vec3 position;
		//depth is a lenth of intersected figure projected on a normal axis
		float depth;
		//
		bool collided;

		CollisionEvent(vec3 normal, vec3 position, float depth, bool collided)
		{
			this->normal = normal;
			this->position = position;
			this->depth = depth;
			this->collided = collided;
		}

		EVENT_CATEGORY(PHYSIC_EVENTS);
		EVENT_TYPE(COLLISION);
	};
}