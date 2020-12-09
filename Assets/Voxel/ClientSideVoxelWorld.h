#pragma once
#include "../../Graphics/Shader/ShaderProgram.h"
#include "ChunkNode.h"
#include "VoxelWorld.h"
#include <queue>
using namespace TealEngine;
class VoxelData;

enum ClientSideVoxelWorldEvents
{
	CHUNK_DATA_RECIVE,
	CHUNK_DATA_MODIFIED
};

class ClientSideVoxelWorld : public VoxelWorld
{
private:
	map<ivec3, ChunkNode*, ivec3Comparator> chunkNodes;
	ShaderProgram* shader;
	vec3 renderPosition;
	std::set<ivec3, ivec3Comparator> meshCreatingQueue;
	unsigned int loadsPerFrame;
	int renderDistance;
	float lastload;

	void messageDispatch(Event* e);

public:
	ClientSideVoxelWorld(int renderDistance, ivec3 chunkSize, ShaderProgram* shader, unsigned int loadsPerFrame = 1);
	void loadChunk(ivec3 index, Voxel* data, unsigned int* xLayerMass);
	void unloadChunk(ivec3 index);
	void setRenderPosition(vec3 position);
	void update();
};