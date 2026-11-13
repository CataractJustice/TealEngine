#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"
#include "Math/linearSolve.h"
#include "Core.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::SphereToScalarField(SphereCollider* sc, ScalarFieldCollider* sfc)
	{
		glm::vec3 spos = sc->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		float stepSize = sc->radius / 12.0f;
		if(stepSize <= 0.0f) stepSize = 0.0001f;
		glm::vec3 collisionPointsSum = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);

		std::vector<glm::vec3> collisions;

		for(float i = -sc->radius; i <= sc->radius; i += stepSize) 
		{	
			for(float j = -sc->radius; j <= sc->radius; j += stepSize) 
			{
				{
					glm::vec3 p = glm::normalize(glm::vec3(i, j, sc->radius)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
				{
					glm::vec3 p = glm::normalize(glm::vec3(i, j, -sc->radius)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
				{
					glm::vec3 p = glm::normalize(glm::vec3(i, sc->radius, j)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
				{
					glm::vec3 p = glm::normalize(glm::vec3(i, -sc->radius, j)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
				{
					glm::vec3 p = glm::normalize(glm::vec3(sc->radius, i, j)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
				{
					glm::vec3 p = glm::normalize(glm::vec3(-sc->radius, i, j)) * sc->radius + spos;
					float val = sfc->getScalarFieldAt(p);
					if(val > 0.0f) 
					{
						collisions.push_back(p);
						normal += sfc->getNormalAt(p);
					}
				}
			}
		}
		float depth = -1000000.0f;
		normal = glm::normalize(normal);
		for(glm::vec3& collPoint : collisions) 
		{
			const float depthSampleStep = 0.01f;
			float py0 = sfc->getScalarFieldAt(collPoint);
			float py1 = sfc->getScalarFieldAt(collPoint + normal * depthSampleStep);
			depth = glm::max(depth, solveLinearEquationFromTwoPoints(0.0f, py0, depthSampleStep, py1, 0.0f));
			collisionPointsSum += collPoint;
		}
		collisionPointsSum /= collisions.size();
		Collision colla, collb;

		colla.normal = -normal;
		colla.depth = depth;
		colla.collidedWith = sfc;
		colla.collider = sc;
		colla.pos = spos + collisionPointsSum;
		colla.relativePos = collisionPointsSum;

		collb.collidedWith = sc;
		collb.collider = sfc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.pos = colla.pos;
		collb.relativePos = glm::vec3(0.0f);

		return { colla, collb };
	}
}