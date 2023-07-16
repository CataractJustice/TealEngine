#pragma once
#include "Collision.h"
#include "Colliders/Colliders.h"
#include "Utils/isPairContains.h"
#include "Utils/isPairInGivenOrder.h"
namespace TealEngine 
{
	class CollisionSolver
	{
	private:
		//returns true if pair of colliders 'a' and 'b' is a pair of colliders with types of firstType and secondType (no mater the order)
		//AND swaps 'a' and 'b' pointers if 'a' type is secondType and 'b' type is firstType
		static bool matchAndSwapColliders(Collider*& a, Collider*& b, Collider::ColliderType firstType, Collider::ColliderType secondType);

	public:
		static std::pair<Collision, Collision> AABCToAAPlane(AABC* aabc, AAPlaneCollider* aapc);

		static std::pair<Collision, Collision> AABCToHeightMap(AABC* aabc, HeightCollider* hc);

		static std::pair<Collision, Collision>SphereToHeightMap(SphereCollider* sc, HeightCollider* hc);

		static std::pair<Collision, Collision>SphereToSphere(SphereCollider* sca, SphereCollider* scb);

		static std::pair<Collision, Collision>BoxToSphere(BoxCollider* bc, SphereCollider* sc);

		static std::pair<Collision, Collision>BoxToBox(BoxCollider* bca, BoxCollider* bcb);

		static std::pair<Collision, Collision> SolveCollision(Collider* a, Collider* b, bool& isAnyCollisionOccured);
	};
}