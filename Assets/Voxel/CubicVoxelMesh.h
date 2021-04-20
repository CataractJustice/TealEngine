#pragma once
#include "VoxelData.h"
#include "../../Graphics/Mesh/Mesh.h"
#include "../../Math/IndexConvert.h"
#include "../../Graphics/Mesh/MeshUtil.h"
#include "../../EventSystem/Misc/Voxel/VoxelDataModifyEvent.h"
class CubicVoxelMesh : public Mesh
{
private:
	VoxelData* data;
	float threshold;
	ivec3 dimensions;
	EventListener modifyListener;
	bool* cellFaces;
	bool modified;

	bool getCellFace(ivec3 pos, int face)
	{
		return cellFaces[a3to1ind(pos.x, pos.y, pos.z, dimensions.y, dimensions.z) * 6 + face];
	}

	void setCellFace(ivec3 pos, int face, bool value)
	{
		cellFaces[a3to1ind(pos.x, pos.y, pos.z, dimensions.y, dimensions.z) * 6 + face] = value;
	}

	void OnModifyCallback(Event* e) 
	{
		
	}

public:
	CubicVoxelMesh(VoxelData* data, float threshold = 0.5f) : Mesh(GL_DYNAMIC_DRAW)
	{

		this->enableAllAttribs();
		this->apply();
		this->data = data;
		this->threshold = threshold;
		dimensions = data->getDimensions();
		cellFaces = new bool[dimensions.x * dimensions.y * dimensions.z * 6];
		eventListenerFunc modifyCallback(std::bind(&CubicVoxelMesh::OnModifyCallback, this, std::placeholders::_1));
		modifyListener = modifyCallback;
		data->onModify.subscribe(&modifyListener);
		modified = false;
	}

	void updateMesh() 
	{
		if (modified)
		{
			modified = false;
			this->clear();
			for (int i = 0; i < dimensions.x; i++)
			{
				for (int j = 0; j < dimensions.y; j++)
				{
					for (int k = 0; k < dimensions.z; k++)
					{
						if (data->getVoxelWrapped(ivec3(i,j,k))->amount > threshold) 
						{
							mat4 t = scale(translate(mat4(1.0f), vec3(i, j, k) + vec3(1.0f)), vec3(0.5f));
							for (int l = 0; l < 6; l++)
							{

								if (getCellFace(ivec3(i, j, k), l) == true)
								{
									addMesh(BasicMeshes::Cube::getCubeFace(BasicMeshes::Cube::CubeFace(l)), t, true);
								}
							}
						}
					}
				}
			}
			this->apply();
		}
	}



	~CubicVoxelMesh() 
	{
		delete[] cellFaces;
	}
};