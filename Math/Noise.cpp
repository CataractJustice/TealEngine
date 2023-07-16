#include "Noise.h"
#include <cmath>
#include "randrange.h"
#include "libs/glm/glm.hpp"

	int Noise::seed = 0;

	float Noise::get(int x, int y, int z)
	{
		int a = (x^(z<<16)) + ((y^(z<<16)) << 16);
		a += 0xe120fc15;
		long tmp = a * 0x4a39b70d;
		int m1 = (tmp >> 32) ^ tmp;
		tmp = ((long)m1) * 0x12fad5c09;
		return ((float)((int)((tmp >> 32) ^ tmp))) / 2147483647.0f; 
	}

	void Noise::setSeed(int seed) 
	{
		Noise::seed = seed;
	}