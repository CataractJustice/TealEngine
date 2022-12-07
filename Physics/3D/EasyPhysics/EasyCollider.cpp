#include "EasyCollider.h"

namespace TealEngine 
{
	namespace Physics 
	{
		namespace EasyPhysics 
		{
			EasyCollider::EasyCollider() : Collider(&EasyPhysicsWorld::instance) 
			{
				radius = 1.0f;
			}
		}
	}
}