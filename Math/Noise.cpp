#include "Noise.h"
#include <cmath>
#include "randrange.h"
#include "libs/glm/glm.hpp"

	int Noise::seed = 0;

	inline unsigned int uintHash(unsigned int n) 
	{
		unsigned int a = (n^(n<<16)) + ((n^(n<<16)) << 16);
		a += 0xe120fc15;
		unsigned int tmp = a * 0x4a39b70d;
		unsigned int m1 = (tmp >> 16) ^ tmp;
		tmp = m1 * 0x12fad5c09;
		return ((tmp >> 16) ^ tmp); 
	}

	inline unsigned int uintHash31(int x, int y, int z) 
	{
		return uintHash(x^(y*0x2f8a91e6) * y^(z*0x17718fad) * z^(x*0x13d4e801));
	}

	float Noise::get(int x, int y, int z)
	{
		return ((float)uintHash31(x,y,z)) / (float)(~((unsigned int)(0)));
	}

	void Noise::setSeed(int seed) 
	{
		Noise::seed = seed;
	}