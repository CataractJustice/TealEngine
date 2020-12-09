#pragma once
#include "../../libs/glm/vec3.hpp"
using namespace glm;
namespace TealEngine 
{
	class AABB
	{
	public:
		vec3 position;
		vec3 scale;

		AABB(vec3 position = vec3(0.0f), vec3 scale = vec3(0.0f)) 
		{
			this->position = position;
			this->scale = scale;
		}

		bool checkIntersection(AABB aabb) 
		{
			return 
				(
					position.x <= aabb.position.x + aabb.scale.x && position.x + scale.x >= aabb.position.x &&
					position.y <=  aabb.position.y + aabb.scale.y && position.y + scale.y >= aabb.position.y &&
					position.z <= aabb.position.z + aabb.scale.z && position.z + scale.z >= aabb.position.z
				);
		}

		bool isPointIn(vec3 point) 
		{
			return (point.x > position.x && point.y > position.y && point.z > position.z && point.x < position.x + scale.x && point.y < position.y + scale.y && point.z < position.z + scale.z);
		}

		float suqareRadius() 
		{
			return (scale.x * scale.x + scale.y * scale.y + scale.z * scale.z) / 2.0f;
		}

		float radius()
		{
			return sqrt(scale.x * scale.x + scale.y * scale.y + scale.z * scale.z) / 2.0f;
		}
	};
}