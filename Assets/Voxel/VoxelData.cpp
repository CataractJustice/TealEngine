#pragma once
#include "VoxelWorld.h"
#include "VoxelData.h"
#include "../../EventSystem/Misc/Voxel/VoxelDataModifyEvent.h"
using namespace TealEngine;

VoxelData::VoxelData(ivec3 dimensions, ivec3 index, VoxelWorld* world, ScalarFieldWrapping wrapMode)
{
	this->neighborsCount = 0;
	this->dimensions = dimensions;
	this->datalength = dimensions.x * dimensions.y * dimensions.z;
	Voxel defVoxel;
	defVoxel.amount = 0.0f;
	defVoxel.blockType = Blocks::getBlockID("air");
	this->defaultValue = defVoxel;
	this->index = index;
	this->wrapMode = wrapMode;
	this->world = world;
	this->voxels = new Voxel[datalength];
	this->xLayerMass = new unsigned int[dimensions.y];
	memset(xLayerMass, 0, sizeof(unsigned int) * dimensions.y);
	memset(voxels, 0, sizeof(Voxel) * datalength);
}

VoxelData::~VoxelData() 
{
	delete[] voxels;
	delete[] xLayerMass;
}

ivec3 VoxelData::getDimensions()
{
	return dimensions;
}

ivec3 VoxelData::getIndex() 
{
	return index;
}

unsigned int VoxelData::getDataLength() 
{
	return this->datalength * sizeof(Voxel);
}

Voxel* VoxelData::getDataArray() 
{
	return voxels;
}

VoxelData* VoxelData::getNeighbor(ivec3 side) 
{
	if (side.x == 0 && side.y == 0 && side.z == 0)
		return nullptr;
	else
		return neighbors[a3to1ind(side.x + 1, side.y + 1, side.z + 1, 3, 3)];
}
void VoxelData::setNeighbor(ivec3 side, VoxelData* data)
{
	if (side.x == 0 && side.y == 0 && side.z == 0)
		return;
	else
	{
		neighbors[a3to1ind(side.x + 1, side.y + 1, side.z + 1, 3, 3)] = data;
		if (data)neighborsCount++;
		else neighborsCount--;
	}
}

unsigned int VoxelData::getNeighborCount() 
{
	return neighborsCount;
}

Voxel* VoxelData::getVoxel(glm::vec3 position)
{
	return &voxels[a3to1ind(position, dimensions.y, dimensions.z)];
}


Blocks::BlockData* VoxelData::getBlockType(glm::ivec3 position)
{
	ivec3 iPos = ivec3(position);
	if (
		iPos.x < 0 || iPos.x >= dimensions.x ||
		iPos.y < 0 || iPos.y >= dimensions.y ||
		iPos.z < 0 || iPos.z >= dimensions.z
		)
	{
		switch (wrapMode)
		{
		case ERROR:
			TE_DEBUG_ERROR("Index out of bound.");
			return 0;
			break;
		case REPEAT:
			iPos %= dimensions;
			break;
		case CLAMP_TO_BORDER:
			clamp(iPos, ivec3(0), dimensions - ivec3(1));
		case CLAMP:
			return 0;
			break;
		case NEIGHBORED_CHUNK:
			ivec3 neighborSide = ivec3(0.0);
			if (iPos.x < 0) neighborSide.x = -1;
			if (iPos.y < 0) neighborSide.y = -1;
			if (iPos.z < 0) neighborSide.z = -1;

			if (iPos.x >= dimensions.x) neighborSide.x = 1;
			if (iPos.y >= dimensions.y) neighborSide.y = 1;
			if (iPos.z >= dimensions.z) neighborSide.z = 1;

			if (getNeighbor(neighborSide))
				return getNeighbor(neighborSide)->getBlockType(iPos - neighborSide * dimensions);
			return 0;
			break;
		}
	}
	return Blocks::getBlockByID(voxels[a3to1ind(iPos.x, iPos.y, iPos.z, dimensions.y, dimensions.z)].blockType);
}

void VoxelData::quickSetVoxel(glm::ivec3 position, Voxel voxel)
{
#ifdef DEBUG
	if (position.x < 0 || position.y < 0 || position.z < 0 || position.x >= dimensions.x || position.y >= dimensions.y || position.z >= dimensions.z)
	{
		TE_DEBUG_ERROR("Index out of chunk bounds. Use modifyVoxel if you need to use wrapping mode.");
	}
#endif
	unsigned int ind = a3to1ind(position, dimensions.y, dimensions.z);
	if (this->voxels[ind].amount > 0 && voxel.amount <= 0) 
		xLayerMass[position.x]--;
	else if (this->voxels[ind].amount <= 0.0f && voxel.amount > 0.0f) 
		xLayerMass[position.x]++;

	this->voxels[ind] = voxel;
}

void VoxelData::setVoxelData(Voxel* values, unsigned int* xLayerMass) 
{
	memcpy(voxels, values, datalength * sizeof(*values));
	memcpy(this->xLayerMass, xLayerMass, dimensions.x * sizeof(*xLayerMass));
}

void VoxelData::setVoxelDataPointer(Voxel* values, bool deleteOldData)
{
	if (deleteOldData) delete[] voxels;
	voxels = values;
}

void VoxelData::modifyVoxel(glm::ivec3 position, Voxel voxel, int method)
{
	//this looks messy lol, but i hope it's not

	//here is how it will modify voxel
	bool adding = method & VoxelModifyMethod::ADD_VOXEL;
	bool breaking = method & VoxelModifyMethod::BREAK_VOXEL;
	bool set = !(adding || breaking);
	bool keepType = method & VoxelModifyMethod::KEEP_TYPE;
	bool keepAmount = method & VoxelModifyMethod::KEEP_AMOUNT;

	//here is required states of modified voxel
	bool ifEmpty = method & VoxelModifyMethod::IF_EMPTY;
	bool ifNotEmpty = method & VoxelModifyMethod::IF_NOT_EMPTY;
	bool ifSame = method & VoxelModifyMethod::IF_SAME_TYPE;
	bool ifNotSame = method & VoxelModifyMethod::IF_NOT_SAME_TYPE;
	bool ifExposed = method & VoxelModifyMethod::IF_EXPOSED;
	bool ifNotExposed = method & VoxelModifyMethod::IF_NOT_EXPOSED;

	//this voxel will be modified
	Voxel* modifiedVoxel = getVoxel(position);
	Voxel oldVoxel = *modifiedVoxel;

	//here is states of modified voxel
	bool isEmpty = this->isEmpty(*modifiedVoxel);
	bool isSameType = modifiedVoxel->blockType == voxel.blockType;
	bool isExposed = 
		this->isEmpty(*getVoxelWrapped(position + ivec3(1, 0, 0))) ||
		this->isEmpty(*getVoxelWrapped(position + ivec3(0, 1, 0))) ||
		this->isEmpty(*getVoxelWrapped(position + ivec3(0, 0, 1))) ||
		this->isEmpty(*getVoxelWrapped(position - ivec3(1, 0, 0))) ||
		this->isEmpty(*getVoxelWrapped(position - ivec3(0, 1, 0))) ||
		this->isEmpty(*getVoxelWrapped(position - ivec3(0, 0, 1)));

	bool ok =
		(isEmpty || !ifEmpty) &&
		(!isEmpty || !ifNotEmpty) &&
		(isSameType || !ifSame) &&
		(!isSameType || !ifNotSame) &&
		(isExposed || !ifExposed) &&
		(!isExposed || ifNotExposed);
	Voxel newVoxel = *modifiedVoxel;
	if (ok) 
	{
		if (set) 
		{
			if (!keepAmount)
				newVoxel.amount = voxel.amount;
			if (!keepType)
				newVoxel.blockType = voxel.blockType;
		}
		else if (adding) 
		{
			if (!keepAmount)
				newVoxel.amount += voxel.amount;
			if (!keepType)
				newVoxel.blockType = voxel.blockType;
		}
		else if (breaking) 
		{
			if(!isEmpty)
				newVoxel.amount += voxel.amount / Blocks::getBlockByID(modifiedVoxel->blockType)->hardness;
		}
		quickSetVoxel(position, newVoxel);
	}
	
	onModify(&VoxelDataModifyEvent(position, oldVoxel, newVoxel));
}
Voxel* VoxelData::getVoxelWrapped(glm::ivec3 position) 
{
	if (
		position.x < 0 || position.x >= dimensions.x ||
		position.y < 0 || position.y >= dimensions.y ||
		position.z < 0 || position.z >= dimensions.z
		)
	{
		switch (wrapMode)
		{
		case ERROR:
			TE_DEBUG_ERROR("Index out of bound.");
			return nullptr;
			break;
		case REPEAT:
			position %= dimensions;
			break;
		case CLAMP_TO_BORDER:
			position = clamp(position, ivec3(0), dimensions - ivec3(1));
		case CLAMP:
			return &defaultValue;
			break;
		case NEIGHBORED_CHUNK:
			ivec3 neighborSide = ivec3(0.0);
			if (position.x < 0) neighborSide.x = -1;
			if (position.y < 0) neighborSide.y = -1;
			if (position.z < 0) neighborSide.z = -1;

			if (position.x >= dimensions.x) neighborSide.x = 1;
			if (position.y >= dimensions.y) neighborSide.y = 1;
			if (position.z >= dimensions.z) neighborSide.z = 1;

			if (getNeighbor(neighborSide))
				return getNeighbor(neighborSide)->getVoxelWrapped(position - neighborSide * dimensions);
			return &defaultValue;
			break;
		}
	}
	return getVoxel(position);
}

bool VoxelData::isXLayerEmpty(unsigned int x)
{
	return xLayerMass[x] == 0;
}

bool VoxelData::isChunkEmpty() 
{
	unsigned int chunkMass = 0;
	for (unsigned int i = 0; i < dimensions.x; i++)
		chunkMass += xLayerMass[i];
	return chunkMass == 0;
}

bool VoxelData::isChunkFilled() 
{
	unsigned int chunkMass = 0;
	for (unsigned int i = 0; i < dimensions.x; i++)
		chunkMass += xLayerMass[i];

	return chunkMass == datalength;
}

bool VoxelData::isEmpty(Voxel voxel) 
{
	return voxel.amount <= 0.0f || Blocks::getBlockByID(voxel.blockType)->propertysBitmap & Blocks::BlockPropertysBitmap::EMPTY;
}

bool VoxelData::isVisible(Voxel voxel) 
{
	return voxel.amount > 0.0f;
}