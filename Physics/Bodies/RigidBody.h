#pragma once
#include "GameNode/Component.h"
#include "Physics/PhysicMaterial.h"
#include "Core.h"
namespace TealEngine 
{
	class RigidBody : public Component
	{
	private:
		glm::vec3 velocity;
		glm::vec3 additionalVelocity;
		glm::vec3 torque;
		glm::vec3 newtorque;
		float radius;
		bool fixRotation;
		
		PhysicMaterial material;

	public:

		void setRadius(float r);

		void setFixedRotation(bool fixed);

		void update();

		glm::vec3 getVelocity();

		void addVelocity(glm::vec3 vel);

		enum class ForceMode 
		{
			IMPULSE,
			ACCELERATION
		};

		void addForce(glm::vec3 force, ForceMode mode);

		void addVelocityAtPos(glm::vec3 pos, glm::vec3 vel);

		inline void setVelocity(glm::vec3 vel) { this->velocity = vel; };

		glm::vec3 getVelocityAtPos(glm::vec3 pos);

		void setTorque(glm::vec3 torque);

		inline glm::vec3 getTorque() { return torque; };

		PhysicMaterial& Material();

		RigidBody();

		void onCollision(const Collision& collision) override;
	};
}