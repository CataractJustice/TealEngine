#include "PhysicsScene.h"
namespace TealEngine {
	const unsigned int maskSize = 64;
	void PhysicsScene::addCollider(Collider* collider) 
	{
		if(collider->collidesWithMask) 
		{
			colliders.insert(collider);
		}
		else 
		{
			nonCollidingColliders.insert(collider);
		}
	}

	void PhysicsScene::removeCollider(Collider* collider) 
	{
		colliders.erase(collider);
		nonCollidingColliders.erase(collider);
	}

	void PhysicsScene::onSceneComponentAdd(Component* component, GameNode* parent) 
	{
		if(dynamic_cast<Collider*>(component)) addCollider((Collider*)component);
	};
	void PhysicsScene::onSceneComponentRemove(Component* component, GameNode* parent) 
	{
		if(dynamic_cast<Collider*>(component)) 
		if(colliders.find((Collider*)component) != colliders.cend()) 
		removeCollider((Collider*)component);
	};

	inline void collisionCheck(std::stack<Collision>& collisions, Collider* colliderA, Collider* colliderB) 
	{
		long long anyCollisionMask = 
		colliderA->collidesWithMask & colliderB->collidableByMask | 
		colliderB->collidesWithMask & colliderA->collidableByMask
		;
		if(anyCollisionMask)
		{
			bool hasCollision = false;
			std::vector<std::pair<Collision, Collision>> collPairVector = CollisionSolver::SolveCollision(colliderA, colliderB, hasCollision);
			if (hasCollision)
			{
				for(const std::pair<Collision, Collision>& collpair : collPairVector) 
				{
					//if first collides with second and second is collidable by first
					if(collpair.first.collider->collidesWithMask & collpair.second.collider->collidableByMask) collisions.push(collpair.first);
					//if second collides with first and first is collidable by second
					if(collpair.second.collider->collidesWithMask & collpair.first.collider->collidableByMask) collisions.push(collpair.second);
				}
			}
		}
	}
	void PhysicsScene::collisionsCheck() 
	{
		
		std::vector<Collider*> nonCollidingUpdateSet;
		std::vector<Collider*> collidingUpdateSet;
		std::vector<Collider*> removedColliders;
		std::vector<Collider*> removedNonCollidingColliders;
		for(auto i = this->colliders.begin(); i != this->colliders.cend(); i++)
		{
			auto& colliderA = *i;
			if(colliderA->isRemovedFromScene()) 
			{
				removedColliders.push_back(colliderA);
				continue;
			}

			if(colliderA->collidesWithMask == 0) 
			{
				nonCollidingUpdateSet.push_back(colliderA);
				continue;
			}
			auto j = i;
			for (j++; j != this->colliders.cend(); j++)
			{
				auto& colliderB = *j; 
				collisionCheck(collisions, colliderA, colliderB);
			}
		}
		
		for(Collider* colliderA : nonCollidingUpdateSet) 
		{
			this->colliders.erase(colliderA);
			this->nonCollidingColliders.insert(colliderA);
		}

		for(Collider* colliderA : nonCollidingColliders) 
		{
			if(colliderA->isRemovedFromScene()) 
			{
				removedNonCollidingColliders.push_back(colliderA);
			}
			if(colliderA->collidesWithMask) collidingUpdateSet.push_back(colliderA);
			for(Collider* colliderB : colliders) 
			{
				collisionCheck(collisions, colliderA, colliderB);
			}
		}

		for(Collider* colliderA : collidingUpdateSet) 
		{
			for(Collider* colliderB : nonCollidingColliders) 
			{
				collisionCheck(collisions, colliderA, colliderB);
			}
			nonCollidingColliders.erase(colliderA);
			colliders.insert(colliderA);
		}

		for(Collider* coll : removedColliders) colliders.erase(coll);
		for(Collider* coll : removedNonCollidingColliders) nonCollidingColliders.erase(coll);
	}

	void PhysicsScene::collisionsFlush() 
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