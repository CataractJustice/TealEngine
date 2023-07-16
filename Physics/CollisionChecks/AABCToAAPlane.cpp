#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::AABCToAAPlane(AABC* aabc, AAPlaneCollider* aapc)
	{
		Collision colla, collb;
		colla.normal = glm::vec3(0.0f);

		glm::vec3 aabcPos = aabc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		glm::vec3 aapcPos = aapc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();

		float bface;
		float pface;
		float* posAxis = nullptr;

		switch (aapc->aligment)
		{
		case AAPlaneCollider::AAPCAxisAligment::XAXISPLANE:
			bface = (aapc->negative ? aabc->to.x : aabc->from.x) + aabcPos.x;
			pface = aapcPos.x;
			colla.normal.x = aapc->negative ? -1.0f : 1.0f;
			posAxis = &aabcPos.x;
			break;
		case AAPlaneCollider::AAPCAxisAligment::YAXISPLANE:
			bface = (aapc->negative ? aabc->to.y : aabc->from.y) + aabcPos.y;
			pface = aapcPos.y;
			colla.normal.y = aapc->negative ? -1.0f : 1.0f;
			posAxis = &aabcPos.y;
			break;
		case AAPlaneCollider::AAPCAxisAligment::ZAXISPLANE:
			bface = (aapc->negative ? aabc->to.z : aabc->from.z) + aabcPos.z;
			pface = aapcPos.z;
			colla.normal.z = aapc->negative ? -1.0f : 1.0f;
			posAxis = &aabcPos.z;
			break;
		default:
			break;
		}

		colla.depth = aapc->negative ? (bface - pface) : (pface - bface);

		*posAxis = bface - (aapc->negative ? -colla.depth : colla.depth);
		colla.pos = aabcPos;
		colla.relativePos = glm::vec3(0.0f);
		colla.collider = aabc;
		colla.collidedWith = aapc;

		*posAxis = bface + (aapc->negative ? -colla.depth : colla.depth) * 2.0f;
		collb = colla;
		collb.pos = aabcPos;
		collb.relativePos = aabcPos - aapcPos;
		collb.normal = -colla.normal;
		collb.collider = aapc;
		collb.collidedWith = aabc;

		return { colla, collb };
	}
}