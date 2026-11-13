#include "Collider.h"
#include "Core.h"
#include "GameNode/Properties/GlobalyNamedLongMask.h"
namespace TealEngine 
{	
	Collider::Collider() 
	{
		this->removedFromScene = false;
		collidesWithMask = 8 - 1;
		collidableByMask = 8 - 1;
		this->addProp(new GlobalyNamedLongMask(&collidesWithMask, Core::physicsScene->layerNames), "Collides with");
		this->addProp(new GlobalyNamedLongMask(&collidableByMask, Core::physicsScene->layerNames), "Collidable by");
	}
}