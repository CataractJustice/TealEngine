#pragma once
#include "GameNode/Entity/Entity.h"
#include "THost.h"
#include "TPacket.h"
#include "GameNode/GameNode.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/EventListener.h"
#include "Assets/TestEntity.h"
#include "GameNode/Prefab/PrefabFactory.h"
#include <functional>
enum MessageType
{
	CREATE_ENTITY_MSG,
	DESTROY_ENTITY_MSG,
	UPDATE_ENTITY_MSG
};

namespace TealEngine 
{
	class Server 
	{
	private:
		std::map<unsigned int, Entity*> entities;
		std::map<unsigned int, unsigned int> peerEntities;
		THost host;
		GameNode* scene;
		EventListener onConnectListener, onReciveListener;
		
		void onConnect(Event* e);

		void sendEntityCreatedMsg(Entity* entity, PeerData peer);

		void onChildAdded(Event* e);

		void onChildRemoved(Event* e);

	public:
		Entity* getPeerEntity(PeerData peer);

		std::map<unsigned int, PeerData> getPeers();

		void setPeerEntity(PeerData peer, Entity* entity);

		void start(unsigned int port, unsigned int connections);

		void update();

		void sendEvent();

		void broadcastEntityData(unsigned int entityId);

		void sendEntityData(Entity* entity, TPacket data, PeerData peer);
	};
}