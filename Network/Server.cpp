#include "Server.h"

namespace TealEngine
{
	void Server::onConnect(Event* e)
	{
		if (e->getType() == PEER_CONNECTED)
		{
			PeerConnectEvent* pce = (PeerConnectEvent*)e;
			for (std::pair<unsigned int, Entity*> entity : entities)
			{
				if (entity.second->isVisibleForPeer(pce->peerData))
				{
					sendEntityCreatedMsg(entity.second, pce->peerData);
				}
			}
		}
	}

	void Server::sendEntityCreatedMsg(Entity* entity, PeerData peer)
	{
		if (entity->isVisibleForPeer(peer))
		{
			TPacket packet;
			packet.push(CREATE_ENTITY_MSG);
			packet.push(entity->getEntityID());
			packet.push(entity->getEntityTypeName());
			host.send(packet, peer);
		}
	}

	void Server::onChildAdded(Event* e)
	{
		if (e->getType() == CHILD_ADDED)
		{
			ChildNodeAddEvent* cnae = (ChildNodeAddEvent*)e;
			Entity* entity = dynamic_cast<Entity*>(cnae->node);
			if (entity)
			{
				entity->setServer(this);
				std::map<unsigned int, PeerData> peers = host.getPeers();
				this->entities[entity->getEntityID()] = entity;
				for (std::pair<unsigned int, PeerData> peer : peers)
				{
					sendEntityCreatedMsg(entity, peer.second);
				}
			}
		}
	}

	void Server::onChildRemoved(Event* e)
	{
		if (e->getType() == CHILD_REMOVED)
		{
			ChildNodeRemoveEvent* cnre = (ChildNodeRemoveEvent*)e;
			Entity* entity = dynamic_cast<Entity*>(cnre->node);
			if (entity)
			{
				entities.erase(entity->getEntityID()); TPacket packet;
				packet.push(DESTROY_ENTITY_MSG);
				packet.push(entity->getEntityID());
				host.broadcast(packet);
			}
		}
	}

	void Server::sendEntityData(Entity* entity, TPacket data, PeerData peer)
	{
		TPacket head;
		head.push(UPDATE_ENTITY_MSG);
		head.push(entity->getEntityID());
		host.send(head + data, peer);
	}

	Entity* Server::getPeerEntity(PeerData peer)
	{
		return this->entities[peerEntities[peer.peerID]];
	}

	std::map<unsigned int, PeerData> Server::getPeers() 
	{
		return host.getPeers();
	}

	void Server::setPeerEntity(PeerData peer, Entity* entity)
	{
		this->peerEntities[peer.peerID] = entity->getEntityID();
	}

	void Server::start(unsigned int port, unsigned int connections)
	{

		onConnectListener = eventListenerBind(&Server::onConnect, this);
		onReciveListener;
		host.host(port, connections);
		host.onConnect.subscribe(&onConnectListener);

		scene = new GameNode();
		scene->addEventListener(CHILD_ADDED, eventListenerBind(&Server::onChildAdded, this));
		scene->addEventListener(CHILD_REMOVED, eventListenerBind(&Server::onChildRemoved, this));
		//scene->addChild(PrefabFactory::createPrefabInstance("TestCube", true, nullptr));
		//scene->addChild(PrefabFactory::createPrefabInstance("VoxelWorld", true, nullptr));
		
	}

	void Server::update()
	{
		host.pollEvents();
		scene->updateAll();
	}

	void Server::sendEvent()
	{

	}

	void Server::broadcastEntityData(unsigned int entityId)
	{

	}
}