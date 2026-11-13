#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"
namespace TealEngine 
{
	std::vector<std::pair<Collision, Collision>> CollisionSolver::AnyToTiles(Collider* any, TilesCollider* tc) 
	{
		std::vector<std::pair<Collision, Collision>> collisionCollection;
		glm::vec3 from;
		glm::vec3 to;
		any->getAABoundingBox(from, to);
		//overestimating here, this is better than underestimating
		from = glm::floor(from - 0.5f);
		to = glm::ceil(to + 0.5f);
		const glm::vec3 gridSize = glm::vec3(1.0f);
		
		for(float x = from.x; x <= to.x; x += gridSize.x) 
		{
			for(float y = from.y; y <= to.y; y += gridSize.y) 
			{
				for(float z = from.z; z <= to.z; z += gridSize.z) 
				{
					//get colliders at a tile within or near bounding box
					std::vector<Collider*> colliderCollection = tc->getCollidersAt(glm::vec3(x, y, z));
					//check collision for each collider in a tile
					for(Collider* colliderB : colliderCollection) 
					{
						bool hasCollided;
						std::vector<std::pair<Collision, Collision>> collisionCheck = CollisionSolver::SolveCollision(any, colliderB, hasCollided);
						if(!hasCollided) continue;
						collisionCollection.insert(collisionCollection.end(), collisionCheck.begin(), collisionCheck.end());
					}
				}
			}
		}

		return collisionCollection;
	}
}