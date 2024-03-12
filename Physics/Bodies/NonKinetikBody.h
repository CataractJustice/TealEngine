#pragma once
#include "GameNode/Component.h"
#include "CollisionEvent.h"
#include "Physics/Collision.h"

namespace TealEngine
{
	class NonKinetikBody : public Component
	{
	public:
	private:
		void onCollision(Collision* e) override
		{
			if (e->getName() == "CollisionEvent")
			{
				CollisionEvent* ce = (CollisionEvent*)e;

				((GameNode3D*)parent)->getRelativeTransform().translate(ce->coll.depth * ce->coll.normal * 0.5f);
			}
		}
	};
}