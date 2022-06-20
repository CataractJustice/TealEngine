#pragma once
#include "GameNode/Component.h"
#include "System/Clock.h"
#include "System/Comparators/ivec3Comparator.h"
#include "VoxelData.h"
#include "Math/i3mod.h"
#include <queue>
using namespace TealEngine;

class VoxelWorld : public Component
{
private:

protected:
	map<ivec3, VoxelData*, ivec3Comparator> chunks;
	ivec3 chunkSize;
public:
	VoxelWorld(ivec3 chunkSize)
	{
		this->chunkSize = chunkSize;
	}

	vec3 localToChunkLocal(ivec3 point)
	{
		return i3mod(point, chunkSize);
	}
	ivec3 localToChunkIndex(ivec3 point)
	{
		ivec3 chunkIndex = point / chunkSize;
		chunkIndex -= ivec3(point.x < 0, point.y < 0, point.z < 0);
		return chunkIndex;
	}
	Voxel getVoxel(vec3 position)
	{
		ivec3 chunkIndex = localToChunkIndex(position);
		ivec3 chunkPos = localToChunkLocal(position);
		if (getChunk(chunkIndex)) return *getChunk(chunkIndex)->getVoxel(chunkPos);
	}
	void modifyVoxel(vec3 position, Voxel voxel, int method)
	{
		ivec3 chunkIndex = localToChunkIndex(position);
		vec3 chunkPos = localToChunkLocal(position);
		if (getChunk(chunkIndex)) getChunk(chunkIndex)->modifyVoxel(chunkPos, voxel, method);
	}

	VoxelData* getChunk(ivec3 index)
	{
		if (chunks.find(index) != chunks.end())
		{
			return chunks[index];
		}
	}
	virtual void loadChunk(ivec3 index, VoxelData* data)
	{
		if (chunks.find(index) == chunks.end())
		{
			VoxelData* newChunk = data;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					for (int k = -1; k <= 1; k++)
					{
						VoxelData* chunk = getChunk(index + ivec3(i, j, k));
						if (chunk && !(i == 0 && j == 0 && k == 0))
						{
							chunk->setNeighbor(ivec3(-i, -j, -k), newChunk);
							newChunk->setNeighbor(ivec3(i, j, k), chunk);
						}
					}
				}
			}
			chunks[index] = newChunk;
		}
	}

	virtual void unloadChunk(ivec3 index)
	{
		VoxelData* delchunk = getChunk(index);
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				for (int k = -1; k <= 1; k++)
				{
					VoxelData* chunk = getChunk(index + ivec3(i, j, k));
					if (chunk && !(i == 0 && j == 0 && k == 0))
					{
						chunk->setNeighbor(ivec3(-i, -j, -k), nullptr);
					}
				}
			}
		}
		delete delchunk;
		chunks.erase(index);
	}
};