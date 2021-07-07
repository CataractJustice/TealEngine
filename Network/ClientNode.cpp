#include "ClientNode.h"
namespace TealEngine
{
	void ClientComponent::dispatchEvent(Event* e)
	{
		MsgReciveEvent* mre = (MsgReciveEvent*)e;
		std::cout << mre->message.getSchema();
		MessageType msgT = *(MessageType*)mre->message[0];
		unsigned int eId;
		switch (msgT)
		{
		case CREATE_ENTITY_MSG:
			eId = *(unsigned int*)mre->message[1];
			this->addEntity(PrefabFactory::createPrefabInstance(mre->message.getString(2), false), eId);
			break;
		case DESTROY_ENTITY_MSG:
			eId = *(unsigned int*)mre->message[1];
			delete entities[eId];
			entities.erase(eId);
			break;
		case UPDATE_ENTITY_MSG:
			eId = *(unsigned int*)mre->message[1];
			if (entities.find(eId) != entities.cend())
			{
				entities[eId]->handleEvent(mre);
			}
			else 
			{
				TE_DEBUG_WARNING("Server tried to update entity #" + to_string(eId) + " but entity with that id was not found.");
			}
			break;
		default:
			break;
		}
	}

	void ClientComponent::addEntity(Entity* e, unsigned int id)
	{
		e->setEntityID(id);
		entities[e->getEntityID()] = e;
		e->setClient(this);
		getParrent()->addChild(static_cast<GameNode*>(e));
	}

	void ClientComponent::update()
	{
		this->socket.pollEvents();
	}

	ClientComponent::ClientComponent(std::string ip, unsigned int port)
	{
		getParrent()->rename("client_root");
		socket.connect(CONNECTION_TIMEOUT, ip, port);
		messageDispatcher = eventListenerBind(&ClientComponent::dispatchEvent, this);
		socket.onRecive.subscribe(&messageDispatcher);
	}

	void ClientComponent::sendEntityData(Entity* entity, TPacket data, unsigned int packetFlags)
	{
		TPacket head;
		head.push(UPDATE_ENTITY_MSG);
		head.push(entity->getEntityID());
		this->socket.send(head + data, packetFlags);
	}
	
}