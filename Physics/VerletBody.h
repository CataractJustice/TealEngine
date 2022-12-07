#pragma once
#include "GameNode/Component.h"

namespace TealEngine 
{
	namespace Physics 
	{
		class VerletBody : public Component
		{
		private:
			glm::vec3 previousPos;
			glm::vec3 acceleration;
		public:
			float drag;
			bool ignoreCollisions;
			inline void addAcceleration(glm::vec3 acc) { acceleration += acc; }
			VerletBody();
			void update() override;
			void onCollision(const Collision& coll) override;
			void onAttach() override;
		};
	}
}