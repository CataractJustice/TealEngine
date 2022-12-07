#include "Noise.h"
#include <cmath>
#include "randrange.h"
#include "libs/glm/glm.hpp"

	int Noise::seed = 0;

	float Noise::get(int x, int y, int z)
	{
		std::srand(seed + (unsigned int)(x * z + z * y - x * y));
		std::rand();
		return randrange(0.0f, 1.0f);
	}

	void Noise::setSeed(int seed) 
	{
		Noise::seed = seed;
	}