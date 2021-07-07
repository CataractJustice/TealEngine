#pragma once
#include "BulletPhysicsConverting.h"

namespace TealEngine
{
	namespace BulletPhysicsConverting
	{
		glm::vec3 vec3BTtoGLM(btVector3 vec)
		{
			return glm::vec3(vec.x(), vec.y(), vec.z());
		}

		btVector3 vec3GLMtoBT(glm::vec3 vec)
		{
			return btVector3(vec.x, vec.y, vec.z);
		}

		Transform& transformBTtoTE(btTransform btT)
		{
			btVector3 btPos = btT.getOrigin();
			btQuaternion btRot = btT.getRotation();
			btVector3 btRotAxis = btRot.getAxis();
			Transform TEtransform;
			TEtransform.setPosition(glm::vec3(btPos.x(), btPos.y(), btPos.z()));
			TEtransform.setRotation(btRot.getAngle() / glm::pi<float>() * 180.0f, glm::vec3(btRotAxis.x(), btRotAxis.y(), btRotAxis.z()));
			return TEtransform;
		}

		btTransform transformTEtoBT(Transform TEtransform)
		{
			btTransform btT;
			glm::vec3 TEpos = TEtransform.getPosition();
			glm::quat TEquat = TEtransform.getRotation();
			btQuaternion btQuat;
			btT.setIdentity();
			btT.setOrigin(btVector3(TEpos.x, TEpos.y, TEpos.z));
			return btT;
		}
	}
}