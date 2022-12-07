#include "Collider.h"
#include "PhysicsWorld.h"
namespace TealEngine
{
	namespace Physics
	{
		Collider::Collider(PhysicsWorld* world) 
		{
			this->colliderCanBeCollided = (1 << 9) - 1;
			this->colliderCollidesWith = (1 << 5) - 1;
			this->canBeSelfCollidedWith = true;
			this->canSelfCollide = false;
			world->add(this);
			this->world = world;
		}

		Collider::~Collider() 
		{
			this->world->remove(this);
		}

		//set layers at which other colliders can detect collision with this collider
		void Collider::setCollidableAtLayer(unsigned char layer, bool value) 
		{ 
			this->colliderCanBeCollided = value ? this->colliderCanBeCollided | (1 << layer) : this->colliderCanBeCollided & ~(1 << layer);
		}
		//set layers at which this collider will check collisions with other colliders
		void Collider::setCollideWithLayer(unsigned char layer, bool value) 
		{
			this->colliderCollidesWith = value ? this->colliderCollidesWith | (1 << layer) : this->colliderCollidesWith & ~(1 << layer);
		}
	}
}