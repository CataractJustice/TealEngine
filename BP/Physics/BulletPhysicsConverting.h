#pragma once
#include "GameNode/Transform.h"
#include <btBulletDynamicsCommon.h>
namespace TealEngine 
{
	namespace BulletPhysicsConverting 
	{
		glm::vec3 vec3BTtoGLM(btVector3 vec);
		btVector3 vec3GLMtoBT(glm::vec3 vec);
		Transform& transformBTtoTE(btTransform btT);
		btTransform transformTEtoBT(Transform TEtransform);
	}
}