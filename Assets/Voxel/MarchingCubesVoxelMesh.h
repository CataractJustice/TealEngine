#pragma once
#include "VoxelData.h"
#include "Graphics/Mesh/Mesh.h"
#include <stack>


class MarchingCubesVoxelMesh : public Mesh
{
private:
	VoxelData* data;
	float threshold;
	unsigned int levelofdetails;
	ivec3 dimensions;

public:
	MarchingCubesVoxelMesh(VoxelData* data, float threshold = 0.01f, unsigned int levelofdetails = 1);
	void updateMesh();
};