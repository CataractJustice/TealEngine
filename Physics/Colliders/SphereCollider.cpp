#include "SphereCollider.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	void SphereCollider::getAABoundingBox(glm::vec3& from, glm::vec3& to) 
	{
		const Transform& t = this->getParentOfType<GameNode3D>()->getWorldTransform();
		glm::vec3 scale = t.getScale();
		glm::vec3 offset = t.getPosition();
		float scaleFactor = glm::max(scale.x, glm::max(scale.y, scale.z));
		from = glm::vec3(-radius * scaleFactor + offset); 
		to = glm::vec3(radius * scaleFactor + offset); 
	};
}