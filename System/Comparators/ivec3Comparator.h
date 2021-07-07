#pragma once
#include "libs/glm/vec3.hpp"
namespace TealEngine 
{
	struct ivec3Comparator 
	{
		bool operator() (const glm::ivec3& left, const glm::ivec3& right) const
		{
			if (left.x < right.x)
				return true;
			if (left.x == right.x) 
			{
				if (left.y < right.y)
					return true;
				if (left.y == right.y)
					return left.z < right.z;
			}
			return false;
		}
	};
}

//i wish there was an easier way
//x > 0 ? (x > 1 ? (x > 2 ? (x > 3 ? (x > 4 ? (x > 5 ? (x > 6 ? (x > 7 ? (x > 8 ? "odd" : "even") : "odd") : "even") : "odd") : "even") : "odd") : "even") : "odd") : "even"