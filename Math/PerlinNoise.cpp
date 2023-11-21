#include "PerlinNoise.h"
float PerlinNoise::fx = 0.0f;
float PerlinNoise::fy = 0.0f;
float PerlinNoise::fz = 0.0f;
float PerlinNoise::n[2][2][2];

float PerlinNoise::get(float x, float y, float z, unsigned int octaves, float persistance, float lacunarity)
{
	fx = glm::fract(x), fy =glm::fract(y), fz = glm::fract(z);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				n[i][j][k] = Noise::get((int)x + i - ((x < 0.0f && fx) ? 1 : 0), (int)y + j - ((y < 0.0f && fy) ? 1 : 0), (int)z + k - ((z < 0.0f && fz) ? 1 : 0));
	if(octaves - 1) 
	{
		float value = smooth3DGridInterpolation(glm::vec3(fx, fy, fz), n[0][0][0], n[1][0][0], n[0][1][0], n[1][1][0], n[0][0][1], n[1][0][1], n[0][1][1], n[1][1][1]) +
		PerlinNoise::get(z*lacunarity, y*lacunarity, x*lacunarity, octaves - 1, persistance, lacunarity) * persistance
		;
		return value / (1.0f + persistance);
	}
	else 
	{
		return smooth3DGridInterpolation(glm::vec3(fx, fy, fz), n[0][0][0], n[1][0][0], n[0][1][0], n[1][1][0], n[0][0][1], n[1][0][1], n[0][1][1], n[1][1][1]);
	}
}