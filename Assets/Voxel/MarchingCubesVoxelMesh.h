#pragma once
#include "VoxelData.h"
#include "../../Graphics/Mesh/Mesh.h"
#include <stack>


class MarchingCubesVoxelMesh : public Mesh
{
private:
	VoxelData* data;
	float* edges;
	float threshold;
	unsigned int levelofdetails;
	ivec3 dimensions;
	
	EventListener modifyListener;
	bool modified;
	std::stack<ivec3> modifiedPoints;
	void OnModifyCallback(Event* e);

	float getEdgeIntersection(ivec3 position, unsigned int edge, unsigned int LOD);
	void setEdgeIntersection(ivec3 position, unsigned int edge, unsigned int LOD, float value);
public:
	MarchingCubesVoxelMesh(VoxelData* data, float threshold = 0.01f, unsigned int levelofdetails = 1);
	void updateMesh();
	void setModified();
};