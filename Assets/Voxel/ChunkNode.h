#pragma once
#include "CubicVoxelMesh.h"
#include "MarchingCubesVoxelMesh.h"
#include "../../Graphics/Mesh/MeshRenderer.h"
#include <string>

class ChunkNode : public GameNode3D 
{
private:
	MarchingCubesVoxelMesh* mesh;
	//RigidBody* body;
	ivec3 index;
public:

	ChunkNode(VoxelData* data, ShaderProgram* shader)
	{
		mesh = new MarchingCubesVoxelMesh(data, 0.00001f, 2);
 		mesh->rename("chunk_mesh");
		Mesh3DRenderer* renderer = (Mesh3DRenderer*)mesh->addChild(new Mesh3DRenderer());
		renderer->rename("chunk_mesh_renderer");
		renderer->setShader(shader);
		index = data->getIndex();
		this->rename("chunk" + to_string(index.x) + "_" + to_string(index.y) + "_" + to_string(index.z));
		this->addChild(mesh);

		//SharedMesh* origin = new SharedMesh(*Resources::getModel("cube.obj")->sharedMesh());
		//MeshRenderer* mr = new Mesh3DRenderer();
		//mr->setShader(shader);
		//origin->addChild(mr);
		//this->addChild(origin);
		


		//Collider* collider = new Collider(mesh);
		//body = new RigidBody(0.0f, glm::vec3(1.0f));
		//body->addChild(collider);
		//this->addChilds(body);
	}

	ivec3 getIndex() { return index; }

	void updateMesh() 
	{
		mesh->setModified();
	}

	void update() 
	{
		mesh->updateMesh();
	}
};