#include "i3mod.h"
glm::ivec3 i3mod(glm::ivec3 a, glm::ivec3 b)
{
	glm::ivec3 m = glm::ivec3(a.x % b.x, a.y % b.y, a.z % b.z);
	if (m.x < 0)
		m.x += b.x;
	if (m.y < 0)
		m.y += b.y;
	if (m.z < 0)
		m.z += b.z;
	return m;
}