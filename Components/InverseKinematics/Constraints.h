#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	void singleAxisHingeConstraint(glm::vec3 origin, float length, glm::vec3 axis, glm::vec3 centerAngle, float angleRange, GameNode3D* constrainedNode);
}