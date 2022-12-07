#include "EasyPhysicsWorld.h"
#include "EasyCollider.h"
#include "GameNode/GameNode3D.h"
#include "System/Comparators/ivec3Comparator.h"
TealEngine::Physics::EasyPhysics::EasyPhysicsWorld TealEngine::Physics::EasyPhysics::EasyPhysicsWorld::instance;

void TealEngine::Physics::EasyPhysics::EasyPhysicsWorld::solveCollisions() 
{
	std::map<glm::ivec3, std::vector<Collider*>, ivec3Comparator> grid;
	const float gridScale = 4.0f;
	for (auto it = colliders.begin(); it != colliders.end(); it++)
	{
		glm::vec3 pos = (*it)->getParrentOfType<GameNode3D>()->getRelativeTransform().getPosition();
		glm::ivec3 gridPos = glm::ivec3(pos / gridScale) - glm::ivec3(pos.x < 0.0f, pos.y < 0.0f, pos.z < 0.0f);

		if (((EasyCollider*)(*it))->radius > gridScale)
		{
			int gridSpan = (int)(((EasyCollider*)(*it))->radius / gridScale) + 1;
			for(int i = -gridSpan + 1; i <= gridSpan; i++)
					for (int k = -gridSpan + 1; k <= gridSpan; k++)
						grid[gridPos + glm::ivec3(i, 0, k)].push_back((*it));
		} 
		else 
		{
			grid[gridPos + glm::ivec3(0, 0, 0)].push_back((*it));
			grid[gridPos + glm::ivec3(1, 0, 0)].push_back((*it));
			grid[gridPos + glm::ivec3(0, 1, 0)].push_back((*it));
			grid[gridPos + glm::ivec3(1, 1, 0)].push_back((*it));
			grid[gridPos + glm::ivec3(0, 0, 1)].push_back((*it));
			grid[gridPos + glm::ivec3(1, 0, 1)].push_back((*it));
			grid[gridPos + glm::ivec3(0, 1, 1)].push_back((*it));
			grid[gridPos + glm::ivec3(1, 1, 1)].push_back((*it));
		}
	}
	for (auto& cell : grid) 
	{
		for (auto it = cell.second.begin(); it != cell.second.end(); it++)
		{
			auto it2 = it;
			for (it2++; it2 != cell.second.end(); it2++)
			{
				bool c1coll = (*it)->canCollideWith(*it2);
				bool c2coll = (*it2)->canCollideWith(*it);
				EasyCollider* c1 = (EasyCollider*)(*(it));
				EasyCollider* c2 = (EasyCollider*)(*(it2));
				GameNode3D* p1 = c1->getParrentOfType<GameNode3D>();
				GameNode3D* p2 = c2->getParrentOfType<GameNode3D>();
				Transform& t1 = p1->getRelativeTransform();
				Transform& t2 = p2->getRelativeTransform();
				if (c1coll || c2coll)
				{
					glm::vec3 d = t1.getPosition() - t2.getPosition();
					float collLen = (c1->radius + c2->radius);
					float sCollLen = collLen * collLen;
					float sLen = d.x * d.x + d.y * d.y + d.z * d.z;

					if (sLen < sCollLen)
					{
						glm::vec3 n = glm::normalize(d);
						glm::vec3 pos1 = t1.getPosition() - n * c1->radius;
						glm::vec3 pos2 = t2.getPosition() + n * c2->radius;
						float len = glm::sqrt(sLen);
						if (c1coll)
						{
							Collision coll1(c1, c2, collLen - len, pos1, n);
							p1->onCollision(coll1);
						}
						if (c2coll) 
						{
							Collision coll2(c2, c1, collLen - len, pos2, -n);
							p2->onCollision(coll2);
						}
					}
				}
			}
		}
	}
}