#include "VecAngleAroundAxis.h"
#include <cmath>

float vecAngleAroundAxis(glm::vec3 v, glm::vec3 axis) 
{
	float projectedX;
	float projectedY;
	if(axis.x == 0.0f && axis.y == 0.0f) 
	{
		projectedX = v.x;
		projectedY = v.y;
	}
	else if(axis.x == 0.0f && axis.z == 0.0f) 
	{
		projectedX = v.x;
		projectedY = v.z;
	}
	else if(axis.y == 0.0f && axis.z == 0.0f) 
	{
		projectedX = v.z;
		projectedY = v.y;
	}
	else 
	{
		axis = glm::normalize(axis);
		glm::vec3 localXAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), axis));
		glm::vec3 localYAxis = glm::normalize(glm::cross(localXAxis, axis));
		projectedX = glm::dot(localXAxis, v);
		projectedY = glm::dot(localYAxis, v);
	}
	return std::atan2(projectedX, projectedY);
}