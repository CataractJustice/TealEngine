#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::SphereToHeightMap(SphereCollider* sc, HeightCollider* hc)
	{
		glm::vec2 step = glm::min(glm::vec2(hc->scale.x, hc->scale.z), glm::vec2(sc->radius / 4.0f)) / 2.0f;
		float rad2 = sc->radius * sc->radius;
		glm::vec3 spos = sc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		glm::vec2 closestp = glm::vec2(spos.x, spos.y);
		float closesth = -INFINITY;
		float actualh = 0.0f;
		for (float i = -sc->radius; i <= sc->radius; i += step.x)
		{
			for (float j = -sc->radius; j <= sc->radius; j += step.y)
			{
				float dst2 = i * i + j * j;
				if (dst2 <= rad2)
				{
					float h = hc->getHeightAtPosition(glm::vec2(spos.x + i, spos.z + j)) + glm::sqrt(rad2 - dst2);
					if (h > closesth)
					{
						closestp = glm::vec2(spos.x + i, spos.z + j);
						closesth = h;
						actualh = hc->getHeightAtPosition(glm::vec2(spos.x + i, spos.z + j));
					}
				}
			}
		}
		Collision colla, collb;

		colla.normal = hc->getNormalAtPosition(closestp, 0.01f);
		colla.depth = sc->radius - glm::dot(spos - glm::vec3(closestp.x, actualh, closestp.y), colla.normal);
		colla.collidedWith = hc;
		colla.collider = sc;
		colla.pos = spos - colla.normal * sc->radius;
		colla.relativePos = colla.pos - spos;

		collb.collidedWith = sc;
		collb.collider = hc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.pos = colla.pos + colla.normal * (sc->radius - collb.depth);
		collb.relativePos = glm::vec3(0.0f);

		return { colla, collb };
	}
}