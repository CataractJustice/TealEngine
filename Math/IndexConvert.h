#pragma once
#include "System/Debug.h"
#include "libs/glm/vec3.hpp"
template <class TI, class TH, class TL>
inline TI a3to1ind(TI x, TI y, TI z, TH h, TL l) 
{
	return z + y * l + x * h * l;
}

template <class T>
inline T a3to1ind(const glm::ivec3& ind, T h, T l)
{
	return ind.z + ind.y * l + ind.x * h * l;
}

template <class TI, class TH, class TL>
inline glm::ivec3 a1to3ind(TI index, TH h, TL l)
{
    TI x = index / (h * l);
    TI rem = index % (h * l);

    TI y = rem / l;
    TI z = rem % l;

    return glm::ivec3(x, y, z);
}

template <class T>
inline T a2to1ind(T x, T y, T h)
{
	return y + x * h;
}