#pragma once
#include "Physics/Collider.h"
#include "GameNode/ComponentProp.h"
#include "libs/glm/gtc/type_ptr.hpp"

namespace TealEngine 
{	
	class AABC : public Collider
	{
	public:

		glm::vec3 from;
		glm::vec3 to;

		AABC() 
		{
			this->from = glm::vec3(-1.0f);
			this->to = glm::vec3(1.0f);
			this->addProp(new FloatVecProp(glm::value_ptr(this->from), 3), "From");
			this->addProp(new FloatVecProp(glm::value_ptr(this->to), 3), "To");
		}

		Collider::ColliderType getColliderType() override
		{
			return ColliderType::AABC;
		}
	};
}
