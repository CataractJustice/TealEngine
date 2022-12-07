#include "VerletBody.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	namespace Physics 
	{
		VerletBody::VerletBody() : acceleration(0.0f), ignoreCollisions(false) {}
		void VerletBody::onAttach() 
		{
			this->previousPos = this->getParrentOfType<GameNode3D>()->getRelativeTransform().getPosition();
		}

		void VerletBody::update() 
		{
			glm::vec3 currPos = this->getParrentOfType<GameNode3D>()->getRelativeTransform().getPosition();
			glm::vec3 velocity = currPos - previousPos + acceleration;
			acceleration = glm::vec3(0.0f);
			previousPos = currPos;
			this->getParrentOfType<GameNode3D>()->getRelativeTransform().translate(velocity * (1.0f - this->drag));
		}

		void VerletBody::onCollision(const Collision& coll) 
		{
			if(!ignoreCollisions)
				this->getParrentOfType<GameNode3D>()->getRelativeTransform().translate(coll.normal * coll.depth / 2.0f);
		}
	}
}