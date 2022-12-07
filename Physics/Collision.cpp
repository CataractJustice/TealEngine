#include "Collision.h"
#include "Collider.h"

namespace TealEngine 
{
	namespace Physics 
	{
		Collision::Collision(Collider* collider, Collider* collidedWith, float depth, glm::vec3 position, glm::vec3 normal) :
			collider(collider),
			collidedWith(collidedWith),
			depth(depth),
			position(position),
			normal(normal)
		{
		
		}
	}
}