#pragma once
#include "THost.h"
#include "TPacket.h"
#include "GameNode/GameNode.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/EventListener.h"
#include <functional>

namespace TealEngine 
{
	typedef uint64_t NodeNetworkIDType;
	typedef uint64_t ComponentNetworkIDType;

	enum class ServerPacketEType {
		BroadcastToNode,
		BroadcastToComponent,
		NewNode,
		RemoveNode,
		NewComponent,
		RemoveComponent
	};

	class Server 
	{
	private:

		std::map<unsigned int, GameNode*> peerEntities;
		THost host;
		GameNode* scene;
		EventListener onConnectListener, onReceiveListener;
		
		void onConnect(Event* e);

		static NodeNetworkIDType getNodeId(GameNode* node);
		static ComponentNetworkIDType getComponentId(Component* component);

	public:
		
		void start(int port, int connections);
		void update();

		//sends messgae to node
		void broadcastToNode(GameNode* node, TPacket& packet);
		//sends message to component
		void broadcastToComponent(Component* component, TPacket& packet);

		void addNodeToNetwork(GameNode* node);
		void removeNodeFromNetwork(GameNode* node);

		void addComonentToNetwork(Component* component);
		void removeComponentFromNetwork(Component* component);
	};
}