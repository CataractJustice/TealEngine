#pragma once
#include "VoxelData.h"
#include "../../Math/PerlinNoise.h"
#include "../../Math/Interpolation.h"
#include "../../Math/randrange.h"
#include "Biome.h"
class TerrainGenerator
{
private:
	std::vector<Biome> biomes;
	Biome currentBiome;
	void setCurrentBiome(VoxelData* chunk) 
	{
		//fancy math should be here
		//like mixing different noises to get index of biome
		currentBiome = biomes[0];
	}

	float landscape(glm::vec3 position) 
	{
		return PerlinNoise::get(position.x / 16.0f, position.y / 16.0f, position.z / 16.0f, 1, 1.0);//currentBiome.getTerrain(position.x, position.y, position.z);
	}

public:
	void addBiome(Biome biome) 
	{
		this->biomes.push_back(biome);
	}

	void generate(VoxelData* chunk) 
	{
		ivec3 chunkSize = chunk->getDimensions();
		vec3 chunkPos = chunk->getIndex() * chunkSize;
		setCurrentBiome(chunk);
		Voxel voxel;
		for (unsigned int i = 0; i < chunkSize.x; i++) 
		{
			for (unsigned int j = 0; j < chunkSize.y; j++)
			{
				for (unsigned int k = 0; k < chunkSize.z; k++)
				{
					voxel.amount = byte(std::max(0.0f,landscape(glm::vec3(i+chunkPos.x,j+chunkPos.y,k+chunkPos.z)) * 255.0f - 96.0f));
					chunk->quickSetVoxel(glm::ivec3(i,j,k),voxel);
				}
			}
		}
	}
};