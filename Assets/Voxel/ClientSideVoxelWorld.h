#pragma once
#include "Graphics/Shader/ShaderProgram.h"
#include "VoxelWorld.h"
#include "GameNode/GameNode3D.h"
#include <queue>
using namespace TealEngine;
class VoxelData;

class ClientSideVoxelWorld : public VoxelWorld
{
private:
	map<ivec3, GameNode3D*, ivec3Comparator> chunkNodes;
	ShaderProgram* shader;
	vec3 renderPosition;
	std::set<ivec3, ivec3Comparator> meshCreatingQueue;
	unsigned int loadsPerFrame;
	int renderDistance;
	float lastload;

public:
	ClientSideVoxelWorld(int renderDistance, ivec3 chunkSize, ShaderProgram* shader, unsigned int loadsPerFrame = 1);
	void loadChunk(ivec3 index, VoxelData* data);
	void unloadChunk(ivec3 index);
	void setRenderPosition(vec3 position);
	void update();
};