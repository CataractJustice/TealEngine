#include "PhysicsScene.h"
namespace TealEngine {
	void PhysicsScene::addColliders(const std::initializer_list<Collider*>& colliders) 
	{
		this->colliders.insert(colliders);
	}

	void PhysicsScene::removeColliders(const std::initializer_list<Collider*>& colliders) 
	{
		for(const auto& collider : colliders)
			this->colliders.erase(collider);
	}

	void PhysicsScene::CollisionCheck() 
	{
		//to-do: add spatial hashing, or something better if possible
		for (auto i = this->colliders.begin(); i != this->colliders.cend(); i++)  
		{
			auto j = i;
			for (j++; j != this->colliders.cend(); j++)
			{
				//to-do: aabb check, or sphere check
				//if (colliders[i]->getColliderAABB().checkIntersection(colliders[j]->getColliderAABB())) 
				{
					bool isAnyCollisionOccured = false;
					std::pair<Collision, Collision> collpair = CollisionSolver::SolveCollision(*i, *j, isAnyCollisionOccured);
					if (isAnyCollisionOccured)
					{
						collisions.push(collpair.first);
						collisions.push(collpair.second);
					}
				}
			}
		}
	}

	void PhysicsScene::CollisionFlush() 
	{
		while (!collisions.empty()) 
		{
			Collision coll = collisions.top();
			if(coll.depth >= 0.0f)
				coll.collider->getParent()->onCollision(coll);
			collisions.pop();
		}
	}
}