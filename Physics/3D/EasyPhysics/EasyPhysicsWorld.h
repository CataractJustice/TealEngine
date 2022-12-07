#pragma once
#include "Physics/PhysicsWorld.h"
namespace TealEngine 
{
	namespace Physics 
	{
		namespace EasyPhysics
		{
			class EasyPhysicsWorld : public PhysicsWorld
			{
			public:
				static EasyPhysicsWorld instance;

				void solveCollisions() override;
			};
		}
	}
}