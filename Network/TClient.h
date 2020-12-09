#pragma once
#include "EventSystem/EventPublisher.h"
#include "EventSystem/NetworkEvents/NetworkEvents.h"
#include "PeerData.h"
#include "System/Debug.h"
#include <queue>
#include <thread>
namespace TealEngine 
{
	class TClient
	{
	private:
		ENetHost* enHost;
		std::queue<Event*> eventsQueue;
		std::thread* serviceThread;

		void serviceThreadMethod() 
		{
			while (enHost) 
			{
				ENetEvent event;
				while (enet_host_service(enHost, &event, 0))
				{
					PeerData pd;
					pd.enPeer = event.peer;
					switch (event.type)
					{
					case ENET_EVENT_TYPE_CONNECT:
						eventsQueue.emplace(new PeerConnectEvent(pd));
						break;
					case ENET_EVENT_TYPE_RECEIVE:
						eventsQueue.emplace(new MsgReciveEvent(pd, TPacket(event.packet->data, event.packet->dataLength)));
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

		void startServiceThread() 
		{
			serviceThread = new thread(&TClient::serviceThreadMethod, this);
		}

	public:
		bool connect(unsigned int timeout, std::string ip, unsigned int port)
		{
			ENetAddress address;
			enet_address_set_host_ip(&address, ip.c_str());
			address.port = port;
			enHost = enet_host_create(NULL, 1, 2, 0, 0);
			enet_host_connect(enHost, &address, 2, 0);
			ENetEvent event;
			
			if ((enet_host_service(enHost, &event, timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)) 
			{
				startServiceThread();
				return true;
			}
			else 
			{
				enet_host_destroy(enHost);
				return false;
			}
		}
		void disconnect() 
		{
			ENetEvent event;
			enet_peer_disconnect(enHost->peers, 0);
			while (enet_host_service(enHost, &event, 0))
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
			enet_peer_reset(enHost->peers);
			delete enHost;
			enHost = nullptr;
		}

		void pollEvents() 
		{
			while (eventsQueue.size())
			{
				Event* event = eventsQueue.front();
				eventsQueue.pop();
				if (event->isInCategory(NETWORK_EVENTS))
				{
					switch (event->getType())
					{
					case PEER_CONNECTED:
						onConnect(event);
						break;
					case MSG_RECIVED:
						onRecive(event);
						break;
					case PEER_DISCONNECTED:
						onDisconnect(event);
						break;
					default:
						break;
					}
				}
				delete event;
			}
		}

		void send(TPacket tpacket, enet_uint32 packetFlags) 
		{
			ENetPacket* packet = tpacket.createENetPacket(packetFlags);
			enet_host_broadcast(enHost, 0, packet);
			enet_host_flush(enHost);
		}
		EventPublisher onConnect;
		EventPublisher onRecive;
		EventPublisher onDisconnect;
	};
}

