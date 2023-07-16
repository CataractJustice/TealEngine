#pragma once
#include "Physics/Collider.h"
#include "libs/glm/gtc/type_ptr.hpp"
namespace TealEngine 
{
	class BoxCollider : public Collider
	{
	private:
		
	public:
		glm::vec3 from;
		glm::vec3 to;

		Collider::ColliderType getColliderType()
		{
			return Collider::ColliderType::BOX;
		}
		
		BoxCollider();
		
		virtual void render(ShaderProgram* shader, unsigned int stages);
	};
}