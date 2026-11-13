#pragma once
#include "CollisionSolver.h"
#include "Collider.h"
#include <stack>
#include <unordered_set>
namespace TealEngine 
{
	class PhysicsScene : public Component
	{
	private:
		std::unordered_set<Collider*> colliders;
		std::unordered_set<Collider*> nonCollidingColliders;
		std::stack<Collision> collisions;
		std::vector<Collider*> globalColliders;
		const float cellSize = 16.0f;
	public:
		std::string layerNames[64];
		void addCollider(Collider* collider);
		void removeCollider(Collider* collider);
		void onSceneComponentAdd(Component* component, GameNode* parent) override;
		void onSceneComponentRemove(Component* component, GameNode* parent) override;
		void collisionsCheck();
		void collisionsFlush();
	};
}