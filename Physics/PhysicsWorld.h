#pragma once
#include <set>
#include "Collider.h"
#include "Body.h"
namespace TealEngine 
{
	namespace Physics 
	{
		class PhysicsWorld 
		{
		protected:
			std::set<Collider*> colliders;
		public:
			void add(Collider* collider);
			void remove(Collider* collider);
			virtual void solveCollisions() = 0;
		};
	}
}