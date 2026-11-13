#include "Connection.h"
#include "enet/enet.h"
#include "GameNode/SceneNode.h"

namespace TealEngine
{
	struct HostData 
	{
		ENetAddress address;
		ENetHost* server;
	};

	struct ClientData 
	{
		ENetAddress address;
		ENetHost* client;
		ENetPeer* peer;
	};

	struct ClientReceivePacketHeader 
	{
		NetworkId handlerComponent;
	};
	struct HostReceivePacketHeader 
	{
		NetworkId handlerComponent;
	};

	Host::Host() 
	{
		this->hostImplData = new HostData();
		this->live = false;
	}

	Host::~Host() 
	{
		if(live)
		close();
		delete hostImplData;
	}

	void Host::host(int port, int connMax) 
	{
		if(live) return;
		this->port = port;
		hostImplData->address.host = ENET_HOST_ANY;
		hostImplData->address.port = port;
		hostImplData->server = enet_host_create(&hostImplData->address, connMax, 1, 0, 0);
		live = true;
	}

	void Host::update(SceneNode* scene) 
	{	
		ENetEvent event;
		while(enet_host_service (this->hostImplData->server, &event, 0)) 
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				{
					PeerInfo* peerInfo = new PeerInfo();
					peerInfo->id = peerIdCounter++;
					event.peer->data = peerInfo;
					this->peers[peerInfo->id] = event.peer;
					scene->onHostPeerConnect(*peerInfo);
				}
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				scene->onHostPeerDisconnect(*(PeerInfo*)event.peer->data);
				delete (PeerInfo*)event.peer->data;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				{
					if(event.packet->dataLength < sizeof(HostReceivePacketHeader)) 
					{
						enet_packet_destroy(event.packet);
						continue;
					}
					HostReceivePacketHeader *header = (HostReceivePacketHeader*)event.packet->data;
					scene->onHostPeerPacket(header->handlerComponent, *((PeerInfo*)event.peer->data), event.packet->data + sizeof(HostReceivePacketHeader), event.packet->dataLength - sizeof(HostReceivePacketHeader));
					enet_packet_destroy(event.packet);
				}
				break;
			default:
				break;
			}
		}
	}

	void Host::send(const NetworkId& toComponentId, PeerInfo& peer, const void* data, int size) const 
	{
		auto peerIt = peers.find(peer.id);
		if(peerIt == peers.cend()) return;
		ENetPeer* enetPeer = (ENetPeer*)peerIt->second;
		ClientReceivePacketHeader header;
		header.handlerComponent = toComponentId;
		ENetPacket* packet = enet_packet_create(nullptr, sizeof(header) + size, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &header, sizeof(header));
		memcpy(packet->data + sizeof(header), data, size);
		enet_peer_send(enetPeer, 0, packet);
	}

	void Host::broadcast(const NetworkId& toComponentId, const void* data, int size) const 
	{
		ClientReceivePacketHeader header;
		header.handlerComponent = toComponentId;
		ENetPacket* packet = enet_packet_create(nullptr, sizeof(header) + size, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &header, sizeof(header));
		memcpy(packet->data + sizeof(header), data, size);
		enet_host_broadcast(this->hostImplData->server, 0, packet);
	}

	void Host::close() 
	{
		if(!live) return;

		enet_host_destroy(this->hostImplData->server);
		this->hostImplData->server = nullptr;

		live = false;
	}

	Client::Client() 
	{
		clientImplData = new ClientData();
		live = false;
	}

	Client::~Client() 
	{
		if(live)
		close();
		delete clientImplData;
	}

	void Client::connect(const std::string& host, int port) 
	{
		if(live) return;
		this->host = host + ":" + std::to_string(port);
		clientImplData->client = enet_host_create(0, 1, 1, 0, 0);
		enet_address_set_host (&clientImplData->address, host.c_str());
		clientImplData->address.port = port;
		clientImplData->peer = enet_host_connect(clientImplData->client, &clientImplData->address, 1, 0);
		live = true;
		connected = false;
		ENetEvent event;
	}

	void Client::close() 
	{
		enet_host_destroy(clientImplData->client);
		enet_peer_reset(clientImplData->peer);
		live = false;
	}

	void Client::send(const NetworkId& toComponentId, const void* data, int size) const
	{
		HostReceivePacketHeader header;
		header.handlerComponent = toComponentId;
		ENetPacket* packet = enet_packet_create(nullptr, sizeof(header) + size, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &header, sizeof(header));
		memcpy(packet->data + sizeof(header), data, size);
		enet_peer_send(clientImplData->peer, 0, packet);
	}

	void Client::update(SceneNode* scene) 
	{
		ENetEvent event;
		while(enet_host_service(clientImplData->client, &event, 0) > 0) 
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				connected = true;
				scene->onClientConnect();
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				{
					if(event.packet->dataLength < sizeof(ClientReceivePacketHeader)) 
					{
						enet_packet_destroy(event.packet);
						continue;
					}
					ClientReceivePacketHeader *header = (ClientReceivePacketHeader*)event.packet->data;
					scene->onClientPacket(header->handlerComponent, event.packet->data + sizeof(ClientReceivePacketHeader), event.packet->dataLength - sizeof(ClientReceivePacketHeader));
					enet_packet_destroy(event.packet);
					break;
				}
			case ENET_EVENT_TYPE_DISCONNECT:
				{
					if(!connected) 
					{
						scene->onClientConnectFail();
					}
					scene->onClientDisconnect();
					connected = false;
					live = false;
					break;
				}
			default:
				break;
			}
		}
	}
} // namespace TealEngine
