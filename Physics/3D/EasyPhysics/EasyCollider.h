#pragma once
#include "GameNode/Component.h"
#include "Physics/Collider.h"
#include "EasyPhysicsWorld.h"
namespace TealEngine 
{
	namespace Physics 
	{
		namespace EasyPhysics
		{
			class EasyCollider : public Collider
			{
			public:
				EasyCollider();
				float radius;
			};
		}
	}
}