#pragma once
#include "libs/glm/glm.hpp"

inline int floatToIndex(float n, float scale = 1.0f, float origin = 0.0f) 
{
	return int(glm::floor((n - origin) / scale));
}

inline int intToIndex(int n, int scale = 1, int origin = 0) 
{
	if(n < 0) n -= scale - 1;
	return (n - origin) / scale;
}

inline glm::ivec3 ivec3ToIndex(glm::ivec3 p, glm::vec3 scale = glm::ivec3(1), glm::ivec3 origin = glm::ivec3(0)) 
{
	return glm::ivec3(intToIndex(p.x, scale.x, origin.x), intToIndex(p.y, scale.y, origin.y), intToIndex(p.z, scale.z, origin.z));
}

inline glm::ivec3 vec3ToIndex(glm::vec3 p, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 origin = glm::vec3(0.0f)) 
{
	return glm::ivec3(floatToIndex(p.x, scale.x, origin.x), floatToIndex(p.y, scale.y, origin.y), floatToIndex(p.z, scale.z, origin.z));
}

inline int realIMod(int a, int b) 
{
	return ((a % b) + b) % b;
}

inline glm::ivec3 ivec3RealMod(glm::ivec3 a, glm::ivec3 b)
{
	return ((a % b) + b) % b;
}