#pragma once
#include "GameNode/GameNode3D.h"
#include "Collider.h"
#include "BulletPhysicsConverting.h"
#include <btBulletDynamicsCommon.h>
namespace TealEngine
{
	class RigidBody : public GameNode3D
	{
	private:
		btRigidBody* btBody;
		btMotionState* motionState;
		btCompoundShape* shape;
		std::vector<Collider*> colliders;
	public:
		Transform& getWorldTransform() override
		{
			return BulletPhysicsConverting::transformBTtoTE(btBody->getWorldTransform());
		}

		void setRelativeTransform(const Transform& transform) override 
		{
			btBody->setWorldTransform(BulletPhysicsConverting::transformTEtoBT(transform));
		}
		GameNode* addChild(GameNode* node);
		void removeChild(GameNode* node);
		RigidBody(float mass = 1.0f, vec3 inertia = glm::vec3(0.0f));
		~RigidBody();
		btRigidBody* getBody();

		
	};
}