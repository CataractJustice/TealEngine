#pragma once
#include "ServerSideVoxelWorld.h"
#include "Network/Server.h"
#include "EventSystem/NetworkEvents/MsgReciveEvent.h"
#include "System/Debug.h"
enum ServerSideVoxelWorldEvents
{
	CHUNK_DATA_SEND,
	CHUNK_DATA_MODIFY
};

void ServerSideVoxelWorld::messageDispatch(Event* e)
{
	if (e->getType() == MSG_RECIVED)
	{
		MsgReciveEvent* mre = (MsgReciveEvent*)e;
		ServerSideVoxelWorldEvents et = mre->message.getFieldValue<ServerSideVoxelWorldEvents>(2);
		switch (et)
		{
		case CHUNK_DATA_MODIFY:
			break;
		default:
			break;
		}
	}
}

void ServerSideVoxelWorld::sendChunkToPeer(PeerData peer, ivec3 chunkIndex)
{
	VoxelData* chunk = getChunk(chunkIndex);
	unsigned int dataLength = chunk->getDataLength();
	ivec3 index = chunk->getIndex();
	TPacket packet;
	packet.push(CHUNK_DATA_SEND);
	packet.push(index.x);
	packet.push(index.y);
	packet.push(index.z);
	packet.push(dataLength, chunk->getDataArray());
	packet.push(chunk->getDimensions().x, chunk->getXLayersMassArray());
	server->sendEntityData(this, packet, peer);
}

void ServerSideVoxelWorld::unloadChunk(ivec3 index)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				VoxelData* chunk = getChunk(index + ivec3(i, j, k));
				if (chunk && !(i == 0 && j == 0 && k == 0))
				{
					chunk->setNeighbor(ivec3(-i, -j, -k), nullptr);
				}
			}
		}
	}
	this->chunks.erase(index);
}

ServerSideVoxelWorld::ServerSideVoxelWorld(glm::ivec3 chunkSize, unsigned int loadingDistance, double chunkLifeTime) : VoxelWorld(chunkSize)
{
	PerlinNoisePreset preset;
	preset.octaveDivisor = 1.2f;
	preset.octaves = 4;
	Biome biome;
	biome.setLandscapePreset(preset);
	generator.addBiome(biome);
	this->loadingDistance = loadingDistance;
	this->chunkSize = chunkSize;
	this->addEventListener(MSG_RECIVED, eventListenerBind(&ServerSideVoxelWorld::messageDispatch, this));
}

void ServerSideVoxelWorld::enqueueChunkLoad(glm::ivec3 chunkIndex, PeerData peer)
{
	ServerSideChunk* chunk = ((ServerSideChunk*)getChunk(chunkIndex));
	if (loadingQueue.find(std::pair<ivec3, PeerData>(chunkIndex, peer)) == loadingQueue.cend())
	{
		if (chunk)
		{
			if (!chunk->isLoadedForPeer(peer))
				loadingQueue.insert(std::pair<ivec3, PeerData>(chunkIndex, peer));
		}
		else
		{
			loadingQueue.insert(std::pair<ivec3, PeerData>(chunkIndex, peer));
		}
	}
	if (chunk) chunk->setLifeTimeFromNow(chunkLifeTime);
}

void ServerSideVoxelWorld::loadQueuedChunk()
{
	if (!loadingQueue.empty())
	{
		std::pair<ivec3, PeerData> chunk = *(loadingQueue.begin());
		if (chunks.find(chunk.first) == chunks.cend())
		{
			ServerSideChunk* newChunk = new ServerSideChunk(ivec3(chunkSize), chunk.first, this, ScalarFieldWrapping::NEIGHBORED_CHUNK);
			generator.generate(newChunk);
			newChunk->setLifeTimeFromNow(chunkLifeTime);
			chunks[chunk.first] = newChunk;
		}
		else
		{
			((ServerSideChunk*)chunks[chunk.first])->setLifeTimeFromNow(chunkLifeTime);
		}

		if (!((ServerSideChunk*)this->getChunk(chunk.first))->isLoadedForPeer(chunk.second))
		{
			this->sendChunkToPeer(chunk.second, chunk.first);
			((ServerSideChunk*)this->getChunk(chunk.first))->loadForPeer(chunk.second);
		}

		loadingQueue.erase(loadingQueue.begin());
	}
}

void ServerSideVoxelWorld::unloadExpiredChunks()
{
	for (std::pair<ivec3, VoxelData*> chunk : chunks)
	{
		if (((ServerSideChunk*)chunk.second)->isChunkExpired())
		{
			unloadChunk(chunk.first);
		}
	}
}

void ServerSideVoxelWorld::loadChunksForPeer(PeerData peer)
{
	glm::vec3 peerPosition = vec3(0.0f);// this->server->getPeerEntity(peer)->getTransform().getPosition();

	for (int i = -loadingDistance; i <= loadingDistance; i++)
	{
		for (int j = -loadingDistance; j <= loadingDistance; j++)
		{
			for (int k = -loadingDistance; k <= loadingDistance; k++)
			{
				glm::ivec3 chunkIndex = ivec3(peerPosition) / ivec3(chunkSize) + glm::ivec3(i, j, k);
				this->enqueueChunkLoad(chunkIndex, peer);

			}
		}
	}
}

void ServerSideVoxelWorld::update()
{
	std::map<unsigned int, PeerData> peers = this->server->getPeers();

	for (std::pair<unsigned int, PeerData> peer : peers)
	{
		this->loadChunksForPeer(peer.second);
	}

	for (byte i = 0; i < 1; i++)
		loadQueuedChunk();
}
