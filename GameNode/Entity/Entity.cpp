#ifndef ENTITY_CPP
#define ENTITY_CPP
#include "Entity.h"
#include "../../System/Debug.h"
namespace TealEngine
{
	unsigned int lastEntityID = 0;

	std::string Entity::getEntityTypeName()
	{
		return this->entityTypeName;
	}

	void Entity::setEntityTypeName(std::string typeName) 
	{
		if (this->entityTypeName.length()) 
		{
			TE_DEBUG_WARNING("Tried set entity type to \"" + typeName + "\" but it's already set to \"" + this->entityTypeName + "\" in \"" + this->getName() + "\".");
		}
		else 
		{
			this->entityTypeName = typeName;
		}
	}

	unsigned int Entity::getEntityID()
	{
		return entityID;
	}

	void Entity::setEntityID(unsigned int id) 
	{
		this->entityID = id;
	}

	bool Entity::isVisibleForPeer(PeerData peer) 
	{
		return true;
	}

	Entity::Entity(unsigned int id, std::string typeName)
	{
		this->entityTypeName = typeName;
		if (id == 0)
		{
			lastEntityID++;
			this->entityID = lastEntityID;
		}
	}

	void Entity::setServer(Server* server) 
	{
		this->server = server;
	}

	void Entity::setClient(ClientComponent* node) 
	{
		this->client = node;
	}
}
#endif