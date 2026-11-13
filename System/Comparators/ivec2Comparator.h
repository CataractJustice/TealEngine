#pragma once
#include "libs/glm/vec2.hpp"
namespace TealEngine
{
	struct ivec2Comparator
	{
		bool operator() (const glm::ivec2& left, const glm::ivec2& right) const
		{
			if (left.x != right.x) return left.x < right.x;
			if (left.y != right.y) return left.y < right.y;
			return false;
		}
	};
}