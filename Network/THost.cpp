#include "THost.h"
#include "enet/enet.h"

namespace TealEngine 
{
	void THost::host(unsigned int port, unsigned int connections)
	{
		enetAddress = new ENetAddress();
		enet_address_set_host_ip((ENetAddress*)enetAddress, "127.0.0.1");
		((ENetAddress*)enetAddress)->port = port;
		enetHost = enet_host_create((ENetAddress*)enetAddress, connections, 2, 0, 0);
		maxPeersCount = connections;
	}
	void THost::pollEvents()
	{
		ENetEvent event;
		while (enet_host_service((ENetHost*)enetHost, &event, 0) > 0)
		{
			PeerData pd;
			pd.enetPeer = event.peer;
			pd.peerID = event.peer->connectID;
			Event* e = new InvokeEvent();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				e = new PeerConnectEvent(pd);
				peers[pd.peerID] = pd;
				onConnect(e);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				e = new MsgReciveEvent(pd, TPacket((uint8_t*)(event.packet->data), event.packet->dataLength));
				onRecive(e);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				e = new PeerDisconnectEvent(pd);
				onDisconnect(e);
				peers.erase(pd.peerID);
				enet_peer_disconnect((ENetPeer*)pd.enetPeer, 0);
				break;
			default:
				break;
			}
			delete e;
		}
	}
	void THost::disconnect(PeerData peer)
	{
		PeerDisconnectEvent e = PeerDisconnectEvent(peer);
		onDisconnect(&e);
		peers.erase(peer.peerID);
		enet_peer_disconnect((ENetPeer*)peer.enetPeer, 0);
	}
	void THost::close()
	{
		for (std::pair<unsigned int, PeerData> peer : peers)
		{
			disconnect(peer.second);
		}
		enet_host_destroy((ENetHost*)enetHost);
	}
	void THost::broadcast(TPacket packet, unsigned int packetFlags)
	{
		ENetPacket* ePacket = (ENetPacket*)packet.createENetPacket(packetFlags);
		enet_host_broadcast((ENetHost*)enetHost, 0, ePacket);
		//enet_packet_destroy(ePacket);
	}
	void THost::send(TPacket packet, PeerData peer, unsigned int packetFlags)
	{
		ENetPacket* ePacket = (ENetPacket*)packet.createENetPacket(packetFlags);
		enet_peer_send(((ENetPeer*)peer.enetPeer), 0, ePacket);
		//enet_packet_destroy(ePacket);
	}

	PeerData THost::getPeer(unsigned int id)
	{
		return peers[id];
	}

	std::map<unsigned int, PeerData> THost::getPeers()
	{
		return peers;
	}

	THost::~THost()
	{
		delete enetAddress;
	}
}