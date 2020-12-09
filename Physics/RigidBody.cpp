#pragma once
#include "Rigidbody.h"
namespace TealEngine
{
	GameNode* RigidBody::addChild(GameNode* node)
	{
		Collider* collider = dynamic_cast<Collider*>(node);
		if (collider)
		{
			this->colliders.push_back((Collider*)node);
			collider->getTransform();
			btTransform shapeTransform;
			shapeTransform.setIdentity();
			Transform nodeTransform = (collider->getTransform());
			vec3 pos = nodeTransform.getPosition();
			quat rotation = nodeTransform.getRotation();
			shapeTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
			shapeTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
			this->shape->addChildShape(shapeTransform, collider->getShape());
		}
		return GameNode::addChild(node);
	}

	void RigidBody::removeChild(GameNode* node)
	{
		if (dynamic_cast<Collider*>(node))
		{
			for (unsigned short i = 0; i < colliders.size(); i++)
			{
				if (colliders[i] == node)
				{
					colliders.erase(colliders.begin() + i);
				}
			}
		}
		GameNode::removeChild(node);
	}

	RigidBody::RigidBody(float mass, vec3 inertia)
	{
		shape = new btCompoundShape();
		motionState = new btDefaultMotionState();
		btBody = new btRigidBody(mass, motionState, shape, btVector3(1.0f, 1.0f, 1.0f));
		btBody->setLinearVelocity(btVector3(inertia.x, inertia.y, inertia.z));
	}

	RigidBody::~RigidBody()
	{
		delete btBody;
		delete motionState;
		delete shape;
	}

	btRigidBody* RigidBody::getBody()
	{
		return btBody;
	}
}