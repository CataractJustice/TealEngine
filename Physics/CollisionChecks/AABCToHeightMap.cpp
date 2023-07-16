#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::AABCToHeightMap(AABC* aabc, HeightCollider* hc)
	{
		Collision colla, collb;
		glm::vec3 aabcPos = aabc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		float h = hc->getHeightAtPosition(glm::vec2(aabcPos.x, aabcPos.z));
		float at = aabcPos.y + aabc->from.y;

		colla.collidedWith = hc;
		colla.collider = aabc;
		colla.depth = h - at;
		colla.normal = hc->getNormalAtPosition(glm::vec2(aabcPos.x, aabcPos.z), 0.01f);
		colla.pos = glm::vec3(aabcPos.x, at, aabcPos.z);
		colla.relativePos = colla.pos - aabcPos;

		collb.collidedWith = aabc;
		collb.collider = hc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.pos = glm::vec3(aabcPos.x, h, aabcPos.z);
		collb.relativePos = glm::vec3(0.0f);

		return { colla, collb };
	}
}