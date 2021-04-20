#pragma once
#include <enet/enet.h>
#include "EventSystem/EventPublisher.h"
#include "EventSystem/NetworkEvents/NetworkEvents.h"
#include "PeerData.h"
#include "TPacket.h"
namespace TealEngine 
{
	class THost
	{
	private:
		ENetAddress enAddress;
		ENetHost* enHost;
		std::map<unsigned int, PeerData> peers;
		unsigned int maxPeersCount;
		unsigned int peersCount;
		unsigned int lastPeerId;
	public:
		void host(unsigned int port, unsigned int connections = 20)
		{
			enet_address_set_host_ip(&enAddress, "127.0.0.1");
			enAddress.port = port;
			enHost = enet_host_create(&enAddress, connections, 2, 0, 0);
			maxPeersCount = connections;
		}
		void pollEvents()
		{
			ENetEvent event;
			while (enet_host_service(enHost, &event, 0) > 0)
			{
				PeerData pd;
				pd.enPeer = event.peer;
				pd.peerID = event.peer->connectID;
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					peers[pd.peerID] = pd;
					onConnect(&PeerConnectEvent(pd));
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					onRecive(&MsgReciveEvent(pd, TPacket((uint8_t*)(event.packet->data), event.packet->dataLength)));
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					onDisconnect(&PeerDisconnectEvent(pd));
					peers.erase(pd.peerID);
					enet_peer_disconnect(pd.enPeer, 0);
					break;
				default:
					break;
				}
			}
		}
		void disconnect(PeerData peer)
		{
			onDisconnect(&PeerDisconnectEvent(peer));
			peers.erase(peer.peerID);
			enet_peer_disconnect(peer.enPeer, 0);
		}
		void close()
		{
			for (std::pair<unsigned int, PeerData> peer : peers) 
			{
				disconnect(peer.second);
			}
			enet_host_destroy(enHost);
		}
		void broadcast(TPacket packet, enet_uint32 packetFlags = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT)
		{
			ENetPacket* ePacket = packet.createENetPacket(packetFlags);
			enet_host_broadcast(enHost, 0, ePacket);
			//enet_packet_destroy(ePacket);
		}
		void send(TPacket packet, PeerData peer, enet_uint32 packetFlags = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT)
		{
			ENetPacket* ePacket = packet.createENetPacket(packetFlags);
			enet_peer_send(peer.enPeer, 0, ePacket);
			//enet_packet_destroy(ePacket);
		}

		PeerData getPeer(unsigned int id) 
		{
			return peers[id];
		}

		std::map<unsigned int, PeerData> getPeers() 
		{
			return peers;
		}

		EventPublisher onDisconnect;
		EventPublisher onConnect;
		EventPublisher onRecive;
		~THost()
		{

		}
	};
}