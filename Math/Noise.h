#pragma once
#include "libs/glm/glm.hpp"

class Noise
{
private:
	static int seed;
	int w, h, l, length;

public:
	static float get(int x, int y = 1, int z = 1);
	static float get(glm::ivec3 p);
	static unsigned int uintGet(int x, int y = 1, int z = 1);
	static unsigned int uintGet(int x, int y, int z, int w);
	static unsigned int uintGet(glm::ivec3 p);
	static void setSeed(int seed);
	static unsigned int uintHash(unsigned int);
};