#pragma once
#include "System/Debug.h"
#include "libs/glm/vec3.hpp"
template <class T>
inline T a3to1ind(T x, T y, T z, T h, T l) 
{
	return z + y * l + x * h * l;
}

template <class T>
inline T a3to1ind(glm::ivec3 ind, T h, T l)
{
	return a3to1ind(ind.x, ind.y, ind.z, h, l);
}

template <class T>
inline T a2to1ind(T x, T y, T h)
{
	return y + x * h;
}