#pragma once
#include "ServerSideChunk.h"
#include "TerrainGenerator.h"
#include <set>
#include "VoxelWorld.h"
#include "System/Comparators/BinaryComparator.h"

enum ServerSideVoxelWorldEvents 
{
	CHUNK_DATA_SEND,
	CHUNK_DATA_MODIFY
};

class ServerSideVoxelWorld : public VoxelWorld
{
private:
	glm::vec3 chunkSize;
	int loadingDistance;
	double chunkLifeTime;
	TerrainGenerator generator;
	std::set<std::pair<ivec3, PeerData>, BinaryComparator> loadingQueue;

	void messageDispatch(Event* e);
	void sendChunkToPeer(PeerData peer, ivec3 chunkIndex);
	void unloadChunk(ivec3 index);

public:
	ServerSideVoxelWorld(glm::ivec3 chunkSize, unsigned int loadingDistance, double chunkLifeTime);
	void enqueueChunkLoad(glm::ivec3 chunkIndex, PeerData peer);
	void loadQueuedChunk();
	void unloadExpiredChunks();
	void loadChunksForPeer(PeerData peer);
	void update();
};