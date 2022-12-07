#include "PhysicsWorld.h"

namespace TealEngine 
{
	namespace Physics 
	{
		void PhysicsWorld::add(Collider* collider) { colliders.insert(collider); }
		void PhysicsWorld::remove(Collider* collider) { colliders.erase(collider); }
	}
}