#pragma once
#include "../../Math/PerlinNoise.h"
class Biome
{
private:
	PerlinNoisePreset landscapeNoise;
	float landHeight;
public:
	void setLandscapePreset(PerlinNoisePreset preset) 
	{
		landscapeNoise = preset;
		landHeight = 0.0f; 
	}

	float getTerrain(float x, float y, float z)
	{
		float landscape = glm::clamp(PerlinNoise::get(landscapeNoise, x, y, z) - y + landHeight, 0.0f, 1.0f);
		float caves = 0.0f;
		float noise = 0.0f;
		return glm::clamp(landscape + noise - caves, 0.0f, 1.0f);
	}
};