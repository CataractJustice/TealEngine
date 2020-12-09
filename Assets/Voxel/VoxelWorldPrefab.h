#pragma once
#include "ClientSideVoxelWorld.h"
#include "ServerSideVoxelWorld.h"
#include "../../Resources.h"
#define VOXEL_WORLD_CHUNK_SIZE 16
#define VOXEL_WORLD_RENDER_DISTANCE 4
class VoxelWorldPrefab
{
public:
	static Entity* create(unsigned int serverSide, TStruct* data)
	{
		if (serverSide)
		{
			ServerSideVoxelWorld* world = new ServerSideVoxelWorld(ivec3(VOXEL_WORLD_CHUNK_SIZE), VOXEL_WORLD_RENDER_DISTANCE, 4.0f);
			world->rename("VoxelWorld");
			return world;
		}
		else
		{
			ShaderProgram* terrainShader = new ShaderProgram(Resources::getMaterial("testShader"));
			ClientSideVoxelWorld* world = new ClientSideVoxelWorld(VOXEL_WORLD_RENDER_DISTANCE, ivec3(VOXEL_WORLD_CHUNK_SIZE), terrainShader, 2);
		}
	}
};