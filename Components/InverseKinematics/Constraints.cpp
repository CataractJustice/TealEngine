#include "Constraints.h"

namespace TealEngine
{	
	void singleAxisHingeConstraint(glm::vec3 origin, float length, glm::vec3 axis, glm::vec3 centerAngle, float angleRange, GameNode3D* constrainedNode) 
	{
		Transform& transform = constrainedNode->getRelativeTransform();
		const glm::vec3 crossProd = glm::normalize(glm::cross(axis, centerAngle));
		const glm::vec3 delta = transform.getPosition() - origin;
		const glm::vec3 projectedDelta = delta - axis * glm::dot(delta, axis);
		const float projectedDeltaLength = glm::length(projectedDelta);
		if(projectedDeltaLength < 1e-8f) return;//throw std::runtime_error("singleAxisHingeConstraint rojectedDeltaLength is close to 0");
		const glm::vec3 projectedDir = projectedDelta / projectedDeltaLength;
		const float angle = glm::acos(glm::dot(projectedDir, centerAngle));
		const float angleDelta = glm::max(angle - angleRange, 0.0f) * glm::sign(glm::dot(crossProd, projectedDelta));
		const glm::vec3 newDirection = (glm::abs(angleDelta) > 1e-8f) ? (
			angleDelta > 0.0f ? 
			(centerAngle * glm::cos(angleRange) + crossProd * glm::sin(angleRange)) 
			: 
			(centerAngle * glm::cos(-angleRange) + crossProd * glm::sin(-angleRange)) 
		) : projectedDir;
		const glm::vec3 newPosition = origin + newDirection * length;
		transform.lookAt(newDirection, axis);
		transform.setPosition(newPosition);
	}
} // namespace TealEngine
