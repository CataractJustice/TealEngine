#include "RigidBody.h"
#include "GameNode/ComponentProp.h"
#include "libs/glm/gtc/type_ptr.hpp"
#include "Core.h"
#include "Graphics/Renderer/Shapes/Line3D.h"
namespace TealEngine 
{
	void RigidBody::setRadius(float r)
	{
		this->radius = r;
	}

	void RigidBody::setFixedRotation(bool fixed)
	{
		fixRotation = fixed;
	}

	void RigidBody::update()
	{
		velocity += additionalVelocity;
		torque = newtorque;
		additionalVelocity = vec3(0.0f);
		this->getParentOfType<GameNode3D>()->getRelativeTransform().translate(velocity);
		if (!fixRotation && length(torque))
			this->getParentOfType<GameNode3D>()->getRelativeTransform().globalRotate(-length(torque) * 2.0 * glm::pi<float>(), torque);
		else
			torque = glm::vec3(0.0f);
		//addVelocity(-glm::vec3(0.0f, 0.00025f, 0.0f));
	}

	glm::vec3 RigidBody::getVelocity()
	{
		return velocity;
	}

	void RigidBody::addVelocity(glm::vec3 vel)
	{
		additionalVelocity += vel;
	}

	void RigidBody::addForce(glm::vec3 force, ForceMode mode)
	{
		switch (mode)
		{
		case TealEngine::RigidBody::ForceMode::IMPULSE:
			addVelocity(force / Material().mass);
			break;
		case TealEngine::RigidBody::ForceMode::ACCELERATION:
			addVelocity(force / Material().mass);
			break;
		default:
			break;
		}
	}

	void RigidBody::addVelocityAtPos(glm::vec3 pos, glm::vec3 vel) 
	{
		glm::vec3 initialPointVelocity = getVelocityAtPos(pos);
		glm::vec3 targetPointVelocity = initialPointVelocity + vel;

		if (fixRotation || (vel.x == 0 && vel.y == 0 && vel.z == 0))
		{
			additionalVelocity += vel;
			return;
		}

		glm::vec3 lv = pos - this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		
		glm::vec3 initialPointTorqueVelocity = getVelocityAtPos(pos) - getVelocity();
		glm::vec3 newPointTorqueVelocity = initialPointTorqueVelocity + (vel - dot(vel, glm::normalize(lv)) * glm::normalize(lv)) * glm::exp(-glm::length(lv));

		if(abs(glm::dot(glm::normalize(-lv), glm::normalize(newPointTorqueVelocity))) > 0.998f || (newPointTorqueVelocity.x == 0.0f && newPointTorqueVelocity.y == 0.0f && newPointTorqueVelocity.z == 0.0f))
		{
			additionalVelocity += vel;
			return;
		}
	
		if (torque.x == 0.0f && torque.y == 0.0f && torque.z == 0.0f)
			newtorque = glm::normalize(glm::cross(newPointTorqueVelocity, lv)) * length(newPointTorqueVelocity);
		else
		{
			glm::vec3 newTorqueAxis = glm::normalize(glm::cross(newPointTorqueVelocity, lv));
			glm::vec3 lerpedTorqueAxis = glm::normalize(glm::normalize(newTorqueAxis) * length(vel) + glm::normalize(torque) * length(initialPointTorqueVelocity));
			float revolutionPerimeter = glm::abs(glm::dot(lv, glm::normalize(glm::cross(glm::cross(lerpedTorqueAxis, lv), lerpedTorqueAxis))) * 2.0 * glm::pi<float>());
			newtorque = lerpedTorqueAxis * length(newPointTorqueVelocity) / revolutionPerimeter;
		}

		glm::vec3 newPointVelocity;

		additionalVelocity += targetPointVelocity - velocity - newPointTorqueVelocity;
	}

	glm::vec3 RigidBody::getVelocityAtPos(glm::vec3 pos)
	{
		if (fixRotation)
			return getVelocity();
		glm::vec3 lv = pos - this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		float revolutionPerimeter = glm::abs(glm::dot(lv, glm::normalize(glm::cross(glm::cross(lv, torque), torque))) * 2.0 * glm::pi<float>());
		glm::vec3 tvel = glm::normalize(glm::cross(lv, torque)) * length(torque) * revolutionPerimeter;
		if (!isnan(tvel.x) && !isnan(tvel.y) && !isnan(tvel.z))
		{
			Core::shapesRenderer.pushShape(Line3D(pos, pos + getVelocity() + tvel, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.01f));
			return getVelocity() + tvel;
		}
		else
		{
			Core::shapesRenderer.pushShape(Line3D(pos, pos + getVelocity(), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.01f));
			return getVelocity();
		}

	}

	void RigidBody::setTorque(glm::vec3 torque)
	{
		newtorque = torque;
	}

	PhysicMaterial& RigidBody::Material()
	{
		return material;
	}

	RigidBody::RigidBody() : velocity(0.0f), additionalVelocity(0.0f), torque(0.0f), fixRotation(false), radius(1.0f), newtorque(0.0f)
	{
		this->addProp(new FloatVecProp(glm::value_ptr(velocity), 3), "Velocity");
		this->addProp(new FloatVecProp(glm::value_ptr(newtorque), 3), "Torque");
		this->addProp(new BoolProp(&fixRotation), "Fix rotation");
		this->addProp(new FloatProp(&this->material.mass), "Mass");
		this->addProp(new FloatProp(&this->material.bouncines), "Bouncines");
		this->addProp(new FloatProp(&this->material.friction), "Friction");
	}

	void RigidBody::onCollision(const Collision& collision)
	{
		RigidBody* rbody = nullptr;
		glm::vec3 thisCollVel = getVelocityAtPos(collision.pos);
		std::vector<RigidBody*> otherBodies = collision.collidedWith->getParent()->findComponentsByType<RigidBody>();
		if (otherBodies.size())
		{
			rbody = otherBodies[0];
			glm::vec3 rbodyCollVel = rbody->getVelocityAtPos(collision.pos);
			float ua = glm::dot(thisCollVel, -collision.normal);
			float ub = glm::dot(rbodyCollVel, collision.normal);
			float systemspeed = (ua - ub) / 2.0f;
			float iv = ua - systemspeed;
			float C = (Material().bouncines + rbody->Material().bouncines) * 0.5f;
			float ma = Material().mass;
			float mb = rbody->Material().mass;
			float nv = (ma - mb - mb * C) / (ma + mb) * iv;
			if (iv >= 0.0f)
			{
				addVelocityAtPos(collision.pos, collision.normal * (iv - nv));
				glm::vec3 frictionvec = glm::normalize(glm::cross(glm::cross(collision.normal, thisCollVel - rbodyCollVel), collision.normal));
				if (!isnan(frictionvec.x) && !isnan(frictionvec.y) && !isnan(frictionvec.z))
				{
					float relativevel = dot(thisCollVel - rbodyCollVel, frictionvec);
					addVelocityAtPos(collision.pos, -frictionvec * relativevel * (1.0f - glm::exp(-(Material().friction + rbody->Material().friction))) * 0.5f);
				}
			}
			
			this->getParentOfType<GameNode3D>()->getRelativeTransform().translate(collision.depth * collision.normal * Material().mass / (rbody->Material().mass + this->Material().mass));
		}
		else
		{
			glm::vec3 frictionvec = glm::normalize(glm::cross(glm::cross(collision.normal, thisCollVel), collision.normal));
			float normalCollVel = glm::dot(thisCollVel, -collision.normal);
			if (normalCollVel > 0.0f)
				addVelocityAtPos(collision.pos, glm::dot(thisCollVel, -collision.normal) * collision.normal * (1.0f + Material().bouncines));

			float mv = glm::dot(thisCollVel, frictionvec);
			if (!isnan(frictionvec.x) && !isnan(frictionvec.y) && !isnan(frictionvec.z)) 
			{
					addVelocityAtPos(collision.pos, -frictionvec * mv * (1.0f - glm::exp(-Material().friction)));
			}

			this->getParentOfType<GameNode3D>()->getRelativeTransform().translate(collision.depth * collision.normal);
		}
	}
}