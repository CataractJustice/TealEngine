#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"
#include "Core.h"
#include "Math/linearSolve.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::BoxToScalarField(BoxCollider* bc, ScalarFieldCollider* sfc)
	{
		Transform bcTransform = bc->getParentOfType<GameNode3D>()->getWorldTransform();
		glm::vec3 cPoint = bcTransform.pointFromLocalToGlobal((bc->from + bc->to) / 2.0f);
		float c = sfc->getScalarFieldAt(cPoint);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec3 localPosition = glm::vec3(0.0f);
		int collidedPointsCount = 0;
		bool collidedPoints[2][2][2] = {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}};
		for(int i = 0; i <= 1; i++)
		for(int j = 0; j <= 1; j++)
		for(int k = 0; k <= 1; k++) 
		{
			glm::vec3 collPos = glm::vec3(
				i ? bc->from.x : bc->to.x,	
				j ? bc->from.y : bc->to.y, 
				k ? bc->from.z : bc->to.z
			);
			glm::vec3 boxpos = bcTransform.getPosition();
			glm::vec3 p = bcTransform.pointFromLocalToGlobal(collPos);
			float val = sfc->getScalarFieldAt(p);

			if(val > 0.0f) 
			{
				normal += sfc->getNormalAt(p);
				localPosition += collPos;
				collidedPointsCount++;
				collidedPoints[i][j][k] = true;
			}
		}

		localPosition /= (float)collidedPointsCount;
		normal /= (float)collidedPointsCount;

		float depth = -100000.0f;
		const float depthSampleStep = 0.01f;

		for(int i = 0; i <= 1; i++)
		for(int j = 0; j <= 1; j++)
		for(int k = 0; k <= 1; k++) 
		{
			if(collidedPoints[i][j][k]) 
			{
				glm::vec3 collPos = glm::vec3(
				i ? bc->from.x : bc->to.x,	
				j ? bc->from.y : bc->to.y, 
				k ? bc->from.z : bc->to.z
				);
				glm::vec3 p = bcTransform.pointFromLocalToGlobal(collPos);
				float py0 = sfc->getScalarFieldAt(p);
				float py1 = sfc->getScalarFieldAt(p + normal * depthSampleStep);
				depth = glm::max(depth, solveLinearEquationFromTwoPoints(0.0f, py0, depthSampleStep, py1, 0.0f));
			}
		}

		Collision colla, collb;

		colla.pos = bcTransform.pointFromLocalToGlobal(localPosition);
		float py0 = sfc->getScalarFieldAt(colla.pos);
		if(py0 > 0.0f) 
		{
			float py1 = sfc->getScalarFieldAt(colla.pos + normal * depthSampleStep);
			depth = glm::max(depth, solveLinearEquationFromTwoPoints(0.0f, py0, depthSampleStep, py1, 0.0f));
		}
		colla.normal = normal;
		float g0 = sfc->getScalarFieldAt(colla.pos);
		float g1 = sfc->getScalarFieldAt(colla.pos + colla.normal * 0.01f);
		depth = glm::min(depth, glm::distance(bc->from, bc->to));

		colla.depth = depth;
		colla.collidedWith = sfc;
		colla.collider = bc;
		colla.relativePos = colla.pos - bcTransform.getPosition();

		collb.collidedWith = bc;
		collb.collider = sfc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.pos = colla.pos;
		collb.relativePos = glm::vec3(0.0f);

		return { colla, collb };
	}
}
