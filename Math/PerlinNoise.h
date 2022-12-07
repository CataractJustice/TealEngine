#pragma once
#include "Noise.h"
#include "Interpolation.h"
#include "IndexConvert.h"
#include "libs/glm/glm.hpp"

struct PerlinNoisePreset
{
public:
	float offset, octaveDivisor;
	unsigned int octaves;
	PerlinNoisePreset* multiplicationNoise;

	PerlinNoisePreset(float offset = 0.0f, float octaveDivisor = 1.0, unsigned int octaves = 1.0, PerlinNoisePreset* multiplicationNoise = nullptr)
	{
		this->offset = offset;
		this->octaveDivisor = octaveDivisor;
		this->octaves = octaves;
		this->multiplicationNoise = multiplicationNoise;
	}
};

class PerlinNoise
{
private:
	static float fx, fy, fz;
	static float n[2][2][2];
public:

	static float get(float x, float y = 0.0f, float z = 0.0f, unsigned int octaves = 1, float octaveDivisor = 1.0f)
	{
		if (octaves > 0) 
		{
			fx = glm::fract(x), fy =glm::fract(y), fz = glm::fract(z);

			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
						n[i][j][k] = Noise::get((int)x + i, (int)y + j, (int)z + k);

			return (smooth3DGridInterpolation(glm::vec3(fx, fy, fz), n[0][0][0], n[1][0][0], n[0][1][0], n[1][1][0], n[0][0][1], n[1][0][1], n[0][1][1], n[1][1][1]) + PerlinNoise::get(-z, x, y, octaves - 1, octaveDivisor)/octaveDivisor)/(1.0f+1.0f/octaveDivisor);
		}
		else 
		{
			return 0.0f;
		}
	}

	static float* createVolume(glm::vec3 offset, glm::ivec3 size, glm::ivec3 scale, float min = 0.0f, float max = 1.0f) 
	{
		float* volume = new float[size.x * size.y * size.z];
		glm::ivec3 nsize = size / scale + 3;
		float* noise = new float[nsize.x * nsize.y * nsize.z];

		for (int i = 0; i < nsize.x; i++) 
		{
			for (int j = 0; j < nsize.y; j++) 
			{
				for (int k = 0; k < nsize.z; k++) 
				{
					noise[a3to1ind(i, j, k, nsize.y, nsize.z)] = Noise::get(i, j, k);
				}
			}
		}

		for (int i = 0; i < size.x; i++)
		{
			for (int j = 0; j < size.y; j++)
			{
				for (int k = 0; k < size.z; k++)
				{
					volume[a3to1ind(i, j, k, size.y, size.z)] = smooth3DGridInterpolation(
						glm::fract(offset + glm::vec3(i, j, k) / glm::vec3(scale)),
						noise[a3to1ind(i / scale.x, j / scale.y, k / scale.z, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x + 1, j / scale.y, k / scale.z, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x, j / scale.y + 1, k / scale.z, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x + 1, j / scale.y + 1, k / scale.z, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x, j / scale.y, k / scale.z + 1, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x + 1, j / scale.y, k / scale.z + 1, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x, j / scale.y + 1, k / scale.z + 1, nsize.y, nsize.z)],
						noise[a3to1ind(i / scale.x + 1, j / scale.y + 1, k / scale.z + 1, nsize.y, nsize.z)]
					) * (max - min) - min;
				}
			}
		}
		delete[] noise;
		return volume;
	}

	static float get(PerlinNoisePreset preset, float x, float y=0.0f, float z=0.0f)
	{
		return get(preset.offset+x, preset.offset+y, preset.offset+z, preset.octaves, preset.octaveDivisor) * (preset.multiplicationNoise ? get(*(preset.multiplicationNoise),x,y,z) : 1.0f);
	}
};