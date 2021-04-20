#pragma once
#include "EventSystem/EventPublisher.h"
#include "EventSystem/NetworkEvents/NetworkEvents.h"
#include "EventSystem/Misc/InvokeEvent.h"
#include "PeerData.h"
#include "TPacket.h"
namespace TealEngine 
{
	class THost
	{
	private:
		void* enetAddress;
		void* enetHost;
		std::map<unsigned int, PeerData> peers;
		unsigned int maxPeersCount;
		unsigned int peersCount;
		unsigned int lastPeerId;
	public:
		void host(unsigned int port, unsigned int connections = 20);
		void pollEvents();
		void disconnect(PeerData peer);
		void close();
		void broadcast(TPacket packet, unsigned int packetFlags = 8);
		void send(TPacket packet, PeerData peer, unsigned int packetFlags = 8);

		PeerData getPeer(unsigned int id);

		std::map<unsigned int, PeerData> getPeers();

		EventPublisher onDisconnect;
		EventPublisher onConnect;
		EventPublisher onRecive;
		~THost();
	};
}