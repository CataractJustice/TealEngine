#pragma once
#include "ClientSideVoxelWorld.h"
#include "MarchingCubesVoxelMesh.h"
#include "Graphics/Mesh/MeshRenderer.h"
using namespace TealEngine;

ClientSideVoxelWorld::ClientSideVoxelWorld(int renderDistance, ivec3 chunkSize, ShaderProgram* shader, unsigned int loadsPerFrame) : VoxelWorld(chunkSize)
{
	this->chunkSize = chunkSize;
	this->renderDistance = renderDistance;
	this->shader = shader;
	this->loadsPerFrame = loadsPerFrame;
	this->lastload = 0.0f;
}

void ClientSideVoxelWorld::loadChunk(ivec3 index, VoxelData* data)
{
	TE_DEBUG_LOG("Chunk(" + to_string(index.x) + "; " + to_string(index.y) + "; " + to_string(index.z) + ") was loaded on client side;");
	VoxelWorld::loadChunk(index, data);

	//if(meshCreatingQueue.find(index) == meshCreatingQueue.cend())
	//	meshCreatingQueue.insert(index);

	if (data->getNeighborCount() >= 25)
		meshCreatingQueue.insert(index);
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				if (i || j || k)
				{
					VoxelData* neighborChunk = getChunk(index + ivec3(i, j, k));
					if (neighborChunk)
					{
						//if (meshCreatingQueue.find(index + ivec3(i, j, k)) == meshCreatingQueue.cend())
						//	meshCreatingQueue.insert(index + ivec3(i, j, k));
						if (neighborChunk->getNeighborCount() >= 25)
							meshCreatingQueue.insert(index + ivec3(i, j, k));
					}
				}
			}
		}
	}
}

void ClientSideVoxelWorld::unloadChunk(ivec3 index)
{
	VoxelWorld::unloadChunk(index);
	delete this->chunkNodes[index];
}

void ClientSideVoxelWorld::setRenderPosition(vec3 position)
{
	this->renderPosition = position;
}

void ClientSideVoxelWorld::update()
{
	for (unsigned int i = 0; i < loadsPerFrame && meshCreatingQueue.size() > 0; i++)
	{
		ivec3 index = *(meshCreatingQueue.begin());
		if (getChunk(index))
		{
			GameNode3D* node = chunkNodes[index] = new GameNode3D();
			Transform chunkTransform = node->getRelativeTransform();
			chunkTransform.translate(index * chunkSize);
			this->getParrent()->addChild(node);
			node->setRelativeTransform(chunkTransform);
			MarchingCubesVoxelMesh* mesh = new MarchingCubesVoxelMesh(getChunk(index));
			node->attachComponent(mesh);
			Mesh3DRenderer* meshRenderer = new Mesh3DRenderer();
			meshRenderer->setMesh(mesh);
			meshRenderer->setShader(shader);
			node->attachComponent(meshRenderer);
			mesh->updateMesh();
		}
		meshCreatingQueue.erase(meshCreatingQueue.begin());
	}
}
