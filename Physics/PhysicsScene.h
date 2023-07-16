#pragma once
#include "CollisionSolver.h"
#include "Collider.h"
#include <stack>
#include <set>
namespace TealEngine 
{
	class PhysicsScene 
	{
	private:
		std::set<Collider*> colliders;

		std::stack<Collision> collisions;
	public:

		void addColliders(const std::initializer_list<Collider*>& colliders);
		void removeColliders(const std::initializer_list<Collider*>& colliders);

		void CollisionCheck();

		void CollisionFlush();
	};
}