#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"
#include "Core.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::SphereToScalarField(SphereCollider* sc, ScalarFieldCollider* sfc)
	{
		glm::vec3 spos = sc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		float stepSize = sc->radius / 12.0f;
		if(stepSize <= 0.0f) stepSize = 0.0001f;
		float rad2 = sc->radius * sc->radius;
		glm::vec3 collisionPointsSum;
		float closestPointDistance = INFINITY;

		for(float i = sc->radius * 0.5f; i <= sc->radius; i += stepSize) 
		{
			for(float j = sc->radius * 0.5f; j <= sc->radius; j += stepSize) 
			{
				for(float k = sc->radius * 0.5f; k <= sc->radius; k += stepSize) 
				{
					float dst2 = (i*i+j*j+k*k);
					if(dst2 > rad2) continue;
					float dst = glm::sqrt(dst2);
					float val = -1.0f;
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(i, j, k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(i,j,k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(-i, j, k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(-i,j,k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(i, -j, k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(i,-j,k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(-i, -j, k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(-i,-j,k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(i, j, -k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(i,j,-k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(-i, j, -k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(-i,j,-k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(i, -j, -k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(i,-j,-k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
					if((val = sfc->getScalarFieldAt(spos + glm::vec3(-i, -j, -k))) > 0.0f) 
					{
						collisionPointsSum += glm::vec3(-i,-j,-k) / dst * val;
						if(closestPointDistance > dst) closestPointDistance = dst;
					}
				}	
			}	
		}

		closestPointDistance = glm::sqrt(closestPointDistance);

		Collision colla, collb;

		colla.normal = -glm::normalize(collisionPointsSum);
		colla.depth = sc->radius - closestPointDistance;
		colla.collidedWith = sfc;
		colla.collider = sc;
		colla.pos = spos - colla.normal * closestPointDistance;
		colla.relativePos = colla.pos - spos;

		collb.collidedWith = sc;
		collb.collider = sfc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.pos = colla.pos;
		collb.relativePos = glm::vec3(0.0f);

		return { colla, collb };
	}
}