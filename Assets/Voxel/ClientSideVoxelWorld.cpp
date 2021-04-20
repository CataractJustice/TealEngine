#pragma once
#include "ClientSideVoxelWorld.h"
using namespace TealEngine;

void ClientSideVoxelWorld::messageDispatch(Event* e)
{
	if (e->getType() == MSG_RECIVED)
	{
		MsgReciveEvent* mre = (MsgReciveEvent*)e;
		ClientSideVoxelWorldEvents et = mre->message.getFieldValue<ClientSideVoxelWorldEvents>(2);
		switch (et)
		{
		case CHUNK_DATA_RECIVE:
		{
			Voxel* data = mre->message.copyArray<Voxel>(6);
			unsigned int* xLayerMass = mre->message.copyArray<unsigned int>(7);
			ivec3 index = ivec3(mre->message.getFieldValue<int>(3), mre->message.getFieldValue<int>(4), mre->message.getFieldValue<int>(5));
			this->loadChunk(index, data, xLayerMass);
			delete[] data;
			delete[] xLayerMass;
			break;
		}
		default:
			break;
		}
	}
}

ClientSideVoxelWorld::ClientSideVoxelWorld(int renderDistance, ivec3 chunkSize, ShaderProgram* shader, unsigned int loadsPerFrame) : VoxelWorld(chunkSize)
{
	this->chunkSize = chunkSize;
	this->renderDistance = renderDistance;
	this->shader = shader;
	this->loadsPerFrame = loadsPerFrame;
	this->rename("voxel_world");
	this->addEventListener(MSG_RECIVED, eventListenerBind(&ClientSideVoxelWorld::messageDispatch, this));
	this->lastload = 0.0f;
}

void ClientSideVoxelWorld::loadChunk(ivec3 index, Voxel* data, unsigned int* xLayerMass)
{
	TE_DEBUG_LOG("Chunk(" + to_string(index.x) + "; " + to_string(index.y) + "; " + to_string(index.z) + ") was loaded on client side;");
	VoxelData* newChunk = new VoxelData(chunkSize, index, this, NEIGHBORED_CHUNK);
	newChunk->setVoxelData(data, xLayerMass);
	VoxelWorld::loadChunk(index, newChunk);

	//if(meshCreatingQueue.find(index) == meshCreatingQueue.cend())
	//	meshCreatingQueue.insert(index);

	if (newChunk->getNeighborCount() >= 25)
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
			ChunkNode* node = chunkNodes[index] = new ChunkNode(getChunk(index), shader);
			Transform chunkTransform = node->getTransform();
			chunkTransform.translate(index * chunkSize);
			this->addChild(node);
			node->setRelativeTransform(chunkTransform);
			node->updateMesh();
		}
		meshCreatingQueue.erase(meshCreatingQueue.begin());
	}
}
