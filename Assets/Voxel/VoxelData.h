#pragma once
#include "../../EventSystem/EventSystem.h"
#include "../../Math/IndexConvert.h"
#include "../../System/Debug.h"
#include "BlockData.h"
#include "../../libs/glm/glm.hpp"
using namespace TealEngine;

class VoxelWorld;


struct Voxel 
{
public:
	char blockType;
	char amount;
};

enum class ScalarFieldWrapping
{
	ERROR,
	DEFAULT_VALUE,
	REPEAT,
	CLAMP_TO_BORDER,
	CLAMP,
	NEIGHBORED_CHUNK
};

enum class VoxelModifyMethod
{
	SET_VOXEL = 0,
	ADD_VOXEL = 1,
	BREAK_VOXEL = 1 << 1,
	KEEP_TYPE = 1 << 2,
	KEEP_AMOUNT = 1 << 3,
	IF_EMPTY = 1 << 4,
	IF_NOT_EMPTY = 1 << 5,
	IF_SAME_TYPE = 1 << 6,
	IF_NOT_SAME_TYPE = 1 << 7,
	IF_EXPOSED = 1 << 8,
	IF_NOT_EXPOSED = 1 << 9
};

class VoxelData
{
private:
	unsigned int neighborsCount;
	Voxel* voxels;
	unsigned int* xLayerMass;
	ivec3 dimensions;
	VoxelData* neighbors[27];
	ivec3 index;
	unsigned int datalength;
public:
	VoxelWorld* world;
	ScalarFieldWrapping wrapMode;
	Voxel defaultValue;

	VoxelData(ivec3 dimensions, ivec3 index = ivec3(0), VoxelWorld* world = nullptr, ScalarFieldWrapping wrapMode = ScalarFieldWrapping::DEFAULT_VALUE);
	~VoxelData();
	ivec3 getDimensions();
	ivec3 getIndex();
	unsigned int getDataLength();
	Voxel* getDataArray();
	unsigned int* getXLayersMassArray() 
	{
		return xLayerMass;
	}
	
	VoxelData* getNeighbor(ivec3 side);
	void setNeighbor(ivec3 side, VoxelData* data);
	unsigned int getNeighborCount();

	Voxel* getVoxel(glm::vec3 position);
	Voxel* getVoxelWrapped(glm::ivec3 position);
	void modifyVoxel(glm::ivec3 position, Voxel voxel, int method);
	Blocks::BlockData* getBlockType(glm::ivec3 position);
	void quickSetVoxel(glm::ivec3 position, Voxel voxel);
	void setVoxelData(Voxel* values, unsigned int* xLayerMass);
	void setVoxelDataPointer(Voxel* values, bool deleteOldData = false);
	bool isXLayerEmpty(unsigned int x);
	bool isChunkEmpty();
	bool isChunkFilled();
	static bool isEmpty(Voxel voxel);
	static bool isVisible(Voxel voxel);
};