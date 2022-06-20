#include "TClient.h"
namespace TealEngine 
{
	void TClient::serviceThreadMethod()
	{
		while (enetHost)
		{
			ENetEvent event;
			while (enet_host_service((ENetHost*)enetHost, &event, 0))
			{
				PeerData pd;
				pd.enetPeer = event.peer;
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					eventsQueue.emplace(new PeerConnectEvent(pd));
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					eventsQueue.emplace(new MsgReciveEvent(pd, TPacket((char*)event.packet->data, event.packet->dataLength)));
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					eventsQueue.emplace(new PeerDisconnectEvent(pd));
					break;
				default:
					break;
				}
			}
		}
		return;
	}

	void TClient::startServiceThread()
	{
		serviceThread = new thread(&TClient::serviceThreadMethod, this);
	}
	bool TClient::connect(unsigned int timeout, std::string ip, unsigned int port)
	{
		ENetAddress address;
		enet_address_set_host_ip(&address, ip.c_str());
		address.port = port;
		enetHost = enet_host_create(NULL, 1, 2, 0, 0);
		enet_host_connect((ENetHost*)enetHost, &address, 2, 0);
		ENetEvent event;

		if ((enet_host_service((ENetHost*)enetHost, &event, timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT))
		{
			startServiceThread();
			return true;
		}
		else
		{
			enet_host_destroy((ENetHost*)enetHost);
			return false;
		}
	}
	void TClient::disconnect()
	{
		ENetEvent event;
		enet_peer_disconnect(((ENetHost*)enetHost)->peers, 0);
		while (enet_host_service((ENetHost*)enetHost, &event, 0))
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				return;
			}
		}
		enet_peer_reset(((ENetHost*)enetHost)->peers);
		delete (ENetHost*)enetHost;
		enetHost = nullptr;
	}

	void TClient::pollEvents()
	{
		while (eventsQueue.size())
		{
			Event* e = eventsQueue.front();
			eventsQueue.pop();
			if (e->isInCategory(NETWORK_EVENTS))
			{
				switch (e->getType())
				{
				case PEER_CONNECTED:
					onConnect(e);
					break;
				case MSG_RECIVED:
					onRecive(e);
					break;
				case PEER_DISCONNECTED:
					onDisconnect(e);
					break;
				default:
					break;
				}
			}
			delete e;
		}
	}

	void TClient::send(TPacket tpacket, unsigned int packetFlags)
	{
		ENetPacket* packet = (ENetPacket*)tpacket.createENetPacket(packetFlags);
		enet_host_broadcast((ENetHost*)enetHost, 0, packet);
		enet_host_flush((ENetHost*)enetHost);
	}
}