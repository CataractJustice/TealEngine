#include "Collider.h"
#include "Core.h"
namespace TealEngine 
{	
	Collider::Collider() 
	{
		Core::physicsScene.addColliders({this});
	}

	Collider::~Collider() 
	{
		Core::physicsScene.removeColliders({this});
	}
}