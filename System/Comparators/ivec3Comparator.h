#pragma once
#include "libs/glm/vec3.hpp"
namespace TealEngine 
{
	struct ivec3Comparator 
	{
		bool operator() (const glm::ivec3& left, const glm::ivec3& right) const
		{
			return	
			left.z < right.z ||
			left.z == right.z && (left.y < right.y ||
			left.y == right.y && left.x < right.x);
		}
	};
}