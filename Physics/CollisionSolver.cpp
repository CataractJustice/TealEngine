#include "CollisionSolver.h"
#include "Collision.h"
#include "Colliders/Colliders.h"
#include "Utils/isPairContains.h"
#include "Utils/isPairInGivenOrder.h"
//returns true if pair of colliders 'a' and 'b' is a pair of colliders with types of firstType and secondType (no mater the order)
//AND swaps 'a' and 'b' pointers if 'a' type is secondType and 'b' type is firstType
namespace TealEngine
{
	bool CollisionSolver::matchAndSwapColliders(Collider*& a, Collider*& b, Collider::ColliderType firstType, Collider::ColliderType secondType)
	{
		if (isPairContains(a->getColliderType(), b->getColliderType(), firstType, secondType))
		{
			if (!isPairInGivenOrder(a->getColliderType(), b->getColliderType(), firstType, secondType))
			{
				std::swap(a, b);
			}
			return true;
		}
		return false;
	}
	std::pair<Collision, Collision> CollisionSolver::SolveCollision(Collider* a, Collider* b, bool& isAnyCollisionOccured) 
	{
		isAnyCollisionOccured = true;
		if (matchAndSwapColliders(a, b, Collider::ColliderType::AABC, Collider::ColliderType::AAPLANE))
		{
			return AABCToAAPlane((AABC*)a, (AAPlaneCollider*)b);
		}
		else
		if (matchAndSwapColliders(a, b, Collider::ColliderType::AABC, Collider::ColliderType::HEIGHTMAP))
		{
			return AABCToHeightMap((AABC*)a, (HeightCollider*)b);
		}
		else
		if (matchAndSwapColliders(a, b, Collider::ColliderType::SPHERE, Collider::ColliderType::HEIGHTMAP))
		{
			return SphereToHeightMap((SphereCollider*)a, (HeightCollider*)b);
		}
		else
		if (matchAndSwapColliders(a, b, Collider::ColliderType::SPHERE, Collider::ColliderType::SPHERE))
		{
			return SphereToSphere((SphereCollider*)a, (SphereCollider*)b);
		}
		else
		if (matchAndSwapColliders(a, b, Collider::ColliderType::BOX, Collider::ColliderType::BOX))
		{
			return BoxToBox((BoxCollider*)a, (BoxCollider*)b);
		}
		else
		if (matchAndSwapColliders(a, b, Collider::ColliderType::BOX, Collider::ColliderType::SPHERE))
		{
			return BoxToSphere((BoxCollider*)a, (SphereCollider*)b);
		}
		isAnyCollisionOccured = false;
	}
}