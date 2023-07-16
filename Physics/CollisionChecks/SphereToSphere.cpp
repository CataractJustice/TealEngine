#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::SphereToSphere(SphereCollider* sca, SphereCollider* scb)
	{
		glm::vec3 posA = sca->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		glm::vec3 posB = scb->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		glm::vec3 n = posA - posB;

		Collision colla, collb;

		colla.collidedWith = scb;
		colla.collider = sca;
		colla.depth = sca->radius + scb->radius - glm::length(n);
		colla.normal = glm::normalize(n);
		colla.relativePos = -colla.normal * sca->radius;
		colla.pos = colla.relativePos + posA;

		collb.collidedWith = sca;
		collb.collider = scb;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.relativePos = -collb.normal * scb->radius;
		collb.pos = posB + collb.relativePos;

		return { colla, collb };
	}
}