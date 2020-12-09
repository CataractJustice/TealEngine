#pragma once
#include <enet/enet.h>
namespace TealEngine
{
	class PeerData
	{
	public:
		ENetPeer* enPeer;
		unsigned int peerID;
	};
}