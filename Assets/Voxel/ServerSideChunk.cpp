#include "ServerSideVoxelWorld.h"
#include "ServerSideChunk.h"
#include "../../System/Clock.h"
ServerSideChunk::ServerSideChunk(glm::ivec3 dimensions, glm::ivec3 index, ServerSideVoxelWorld* world, ScalarFieldWrapping wrapMode) : VoxelData(dimensions, index, (VoxelWorld*)world, wrapMode)
{

}

void ServerSideChunk::unloadForPeer(PeerData peer) 
{
	this->loadedForPeers.erase(peer.peerID);
}

void ServerSideChunk::loadForPeer(PeerData peer) 
{
	this->loadedForPeers.emplace(peer.peerID);
}

bool ServerSideChunk::isChunkExpired()
{
	return this->expireTime < Clock::getTime();
}

void ServerSideChunk::setLifeTimeFromNow(double time)
{
	this->expireTime = Clock::getTime() + time;
}

bool ServerSideChunk::isLoadedForPeer(PeerData peer)
{
	return this->loadedForPeers.find(peer.peerID) != this->loadedForPeers.cend();
}