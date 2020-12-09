#pragma once
#include "VoxelData.h"
#include <set>
class ServerSideVoxelWorld;

class ServerSideChunk : public VoxelData
{
private:
	double expireTime;
	std::set<unsigned int> loadedForPeers;
public:
	ServerSideChunk(glm::ivec3 dimensions, glm::ivec3 index = glm::ivec3(0), ServerSideVoxelWorld* world = nullptr, ScalarFieldWrapping wrapMode = NO_WRAP);
	void setLifeTimeFromNow(double time);
	bool isChunkExpired();
	void unloadForPeer(PeerData peer);
	void loadForPeer(PeerData peer);
	bool isLoadedForPeer(PeerData peer);
};