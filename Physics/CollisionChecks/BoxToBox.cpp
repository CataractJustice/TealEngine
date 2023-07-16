#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"
#include "Core.h"
#include "Graphics/Renderer/Shapes/Line3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::BoxToBox(BoxCollider* bca, BoxCollider* bcb)
	{
		Collision colla, collb;
		Transform& atransform = bca->getParentOfType<GameNode3D>()->getWorldTransform();
		Transform& btransform = bcb->getParentOfType<GameNode3D>()->getWorldTransform();

		glm::vec3 apos, bpos;
		apos = atransform.getPosition();
		bpos = btransform.getPosition();

		glm::vec3 ainblimits[2] = { glm::vec3(INFINITY), glm::vec3(-INFINITY) };
		glm::vec3 binalimits[2] = { glm::vec3(INFINITY), glm::vec3(-INFINITY) };

		glm::vec3 acorner;
		glm::vec3 bcorner;

		glm::vec3 afrom = bca->from * atransform.getScale();
		glm::vec3 ato = bca->to * atransform.getScale();
		glm::vec3 afromtemp = afrom;

		afrom = glm::min(afrom, ato);
		ato = glm::max(afromtemp, ato);

		glm::vec3 bfrom = bcb->from* btransform.getScale();
		glm::vec3 bto = bcb->to * btransform.getScale();
		glm::vec3 bfromtemp = bfrom;

		bfrom = glm::min(bfrom, bto);
		bto = glm::max(bfromtemp, bto);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					acorner.x = i ? afrom.x : ato.x;
					acorner.y = j ? afrom.y : ato.y;
					acorner.z = k ? afrom.z : ato.z;

					bcorner.x = i ? bfrom.x : bto.x;
					bcorner.y = j ? bfrom.y : bto.y;
					bcorner.z = k ? bfrom.z : bto.z;

					glm::vec3 ainbc = atransform.pointFromLocalToTransform(acorner, btransform, true);
					glm::vec3 binac = btransform.pointFromLocalToTransform(bcorner, atransform, true);

					for (int l = 0; l < 3; l++)
					{
						if (ainbc[l] < ainblimits[0][l])
						{
							ainblimits[0][l] = ainbc[l];
						}
						if (ainbc[l] > ainblimits[1][l])
						{
							ainblimits[1][l] = ainbc[l];
						}

						if (binac[l] < binalimits[0][l])
						{
							binalimits[0][l] = binac[l];
						}
						if (binac[l] > binalimits[1][l])
						{
							binalimits[1][l] = binac[l];
						}
					}
				}
			}
		}

		glm::vec3 depths[2][2];

		depths[0][0] = binalimits[1] - afrom;
		depths[0][1] = ato - binalimits[0];
		depths[1][0] = ainblimits[1] - bfrom;
		depths[1][1] = bto - ainblimits[0];
		glm::ivec3 minind = glm::ivec3(0);
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 3; k++)
					if ((depths[i][j][k] < depths[minind.x][minind.y][minind.z]))
						minind = glm::ivec3(i, j, k);
		float depth = depths[minind.x][minind.y][minind.z];
		//x - collider
		//y - sign
		//z - normal
		glm::vec3 localnormal = glm::vec3((minind.z == 0) ? 1.0f : 0.0f, (minind.z == 1) ? 1.0f : 0.0f, (minind.z == 2) ? 1.0f : 0.0f) * (minind.y ? -1.0f : 1.0f);

		glm::vec3 normal = (minind.x ? (btransform.pointFromLocalToGlobal(localnormal, true) - btransform.getPosition()) : (atransform.pointFromLocalToGlobal(localnormal, true) - atransform.getPosition()));

		//glm::vec3 cornerpos = (bbfacecorners[minind.z][minind.y][1 - minind.x] + (minind.x ? apos : bpos));
		//glm::vec3 planepos = cornerpos + normal * depth;
		glm::vec3 fallbackPos = (
			btransform.pointFromLocalToGlobal((glm::max(ainblimits[0], bfrom) + glm::min(ainblimits[1], bto)) * 0.5f, true) + 
			atransform.pointFromLocalToGlobal((glm::max(binalimits[0], afrom) + glm::min(binalimits[1], ato)) * 0.5f, true)
		) * 0.5f;

		int collisionCorners = 0;
		glm::vec3 pos = glm::vec3(0.0f);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					acorner.x = i ? afrom.x : ato.x;
					acorner.y = j ? afrom.y : ato.y;
					acorner.z = k ? afrom.z : ato.z;

					bcorner.x = i ? bfrom.x : bto.x;
					bcorner.y = j ? bfrom.y : bto.y;
					bcorner.z = k ? bfrom.z : bto.z;

					glm::vec3 ainbc = atransform.pointFromLocalToTransform(acorner, btransform, true);
					glm::vec3 binac = btransform.pointFromLocalToTransform(bcorner, atransform, true);

					if(ainbc.x > bfrom.x && ainbc.y > bfrom.y && ainbc.z > bfrom.z && ainbc.x < bto.x && ainbc.y < bto.y && ainbc.z < bto.z)
					{
						collisionCorners++;
						pos += atransform.pointFromLocalToGlobal(acorner, true);
						Core::shapesRenderer.pushShape(Line3D(atransform.pointFromLocalToGlobal(acorner, true), atransform.pointFromLocalToGlobal(acorner, true) + 0.1f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.1f));
					}
					if(binac.x > afrom.x && binac.y > afrom.y && binac.z > afrom.z && binac.x < ato.x && binac.y < ato.y && binac.z < ato.z)
					{
						collisionCorners++;
						pos += btransform.pointFromLocalToGlobal(bcorner, true);
						Core::shapesRenderer.pushShape(Line3D(btransform.pointFromLocalToGlobal(bcorner, true), btransform.pointFromLocalToGlobal(bcorner, true) + 0.1f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.1f));
					}
				}
			}
		}

		if(collisionCorners)
		{
			pos /= (float)collisionCorners;
		}
		else 
		{
			pos = fallbackPos;
		}
		

		colla.collidedWith = bcb;
		colla.collider = bca;
		colla.depth = depth;
		colla.normal = normal * (minind.x ? -1.0f : 1.0f);
		colla.pos = pos;
		colla.relativePos = colla.pos - apos;

		collb.collidedWith = bca;
		collb.collider = bcb;
		collb.depth = depth;
		collb.normal = -colla.normal;
		collb.pos = pos;
		collb.relativePos = collb.pos - bpos;

		return { colla, collb };
	}
}