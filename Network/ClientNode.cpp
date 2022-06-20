#include "ClientNode.h"
#include "Network/PacketTemplates.h"
#include "System/Debug.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine
{
	void ClientComponent::dispatchEvent(Event* e)
	{
		MsgReciveEvent* mre = (MsgReciveEvent*)e;
		
		//TE_DEBUG_INFO("Message recieved by client");

		ServerPacketEType msgT = mre->message.getFieldValue<ServerPacketEType>(0);
		unsigned int eId;

		mre->message.clamp(1, mre->message.getFieldsCount());
		
		TPacket packet;
		uint8_t* packetraw = nullptr;

		switch (msgT)
		{
		case TealEngine::ServerPacketEType::BroadcastToNode:
		{
			NodeNetworkIDType nodeid = mre->message.getFieldValue<NodeNetworkIDType>(PacketTemplates::nodePacketTemplate.getIndexOf("node_id"));
			auto mapnode = nodesIdMap.find(nodeid);
			if (mapnode != nodesIdMap.cend()) 
			{
				int i = PacketTemplates::nodePacketTemplate.getIndexOf("packet");
				packetraw = mre->message.copyArray<uint8_t>(i);
				packet = TPacket((char*)packetraw, mre->message.typeSize(i), false);
				GameNode* node = mapnode->second;
				node->onMessageReceive(packet);
				delete[] packetraw;
			}
			else 
			{
				TE_DEBUG_WARNING("Client received message for non existing node");
			}
		}
			break;
		case TealEngine::ServerPacketEType::BroadcastToComponent:
		{
			NodeNetworkIDType componentid = mre->message.getFieldValue<NodeNetworkIDType>(PacketTemplates::componentPacketTemplate.getIndexOf("component_id"));
			auto mapcomponent = componentsIdMap.find(componentid);
			if (mapcomponent != componentsIdMap.cend())
			{
				int i = PacketTemplates::componentPacketTemplate.getIndexOf("packet");
				packetraw = mre->message.copyArray<uint8_t>(i);
				packet = TPacket((char*)packetraw, mre->message.typeSize(i), false);
				Component* component = mapcomponent->second;
				component->onMessageReceive(packet);
				delete[] packetraw;
			}
			else
			{
				TE_DEBUG_WARNING("Client received message for non existing component");
			}
		}
			break;
		case TealEngine::ServerPacketEType::NewNode:
		{
			NodeNetworkIDType nodeid = mre->message.getFieldValue<NodeNetworkIDType>(PacketTemplates::newNodePacketTemplate.getIndexOf("node_id"));
			GameNode* node = new GameNode3D;
			this->getParrent()->addChild(node);
			nodesIdMap[nodeid] = node;
		}
			break;
		case TealEngine::ServerPacketEType::RemoveNode:
		{
			NodeNetworkIDType nodeid = mre->message.getFieldValue<NodeNetworkIDType>(PacketTemplates::removeNodePacketTemplate.getIndexOf("node_id"));
			auto mapnode = nodesIdMap.find(nodeid);
			if (mapnode != nodesIdMap.cend()) {
				delete[] mapnode->second;
			}
			else 
			{
				TE_DEBUG_WARNING("Client received removal message of non existing node");
			}
		}
			break;
		case TealEngine::ServerPacketEType::NewComponent: 
		{
			ComponentNetworkIDType componentid = mre->message.getFieldValue<ComponentNetworkIDType>(PacketTemplates::newComponentPacketTemplate.getIndexOf("component_id"));
			std::string componentFactoryName = mre->message.getString(PacketTemplates::newComponentPacketTemplate.getIndexOf("component_type"));
			Component* component = ComponentFactory::instantiateComponent(componentFactoryName);
			NodeNetworkIDType nodeid = mre->message.getFieldValue<NodeNetworkIDType>(PacketTemplates::newComponentPacketTemplate.getIndexOf("node_id"));
			auto mapnode = nodesIdMap.find(nodeid);
			if (mapnode != nodesIdMap.cend()) {
				componentsIdMap[componentid] = component;
				mapnode->second->attachComponent(component);
			}
			else
			{
				TE_DEBUG_WARNING("Client received message that tries to attach component to non existing node");
			}
		}
			break;
		case TealEngine::ServerPacketEType::RemoveComponent:
		{
			ComponentNetworkIDType componentid = mre->message.getFieldValue<ComponentNetworkIDType>(PacketTemplates::removeComponentPacketTemplate.getIndexOf("component_id"));
			auto mapcomponent = componentsIdMap.find(componentid);
			if (mapcomponent != componentsIdMap.cend()) {
				delete mapcomponent->second;
			}
			else
			{
				TE_DEBUG_WARNING("Client received removal message of non existing component");
			}
		}
			break;
			TE_DEBUG_WARNING("Client received unknown message type");
		default:
			break;
		}
	}

	void ClientComponent::update()
	{
		this->socket.pollEvents();
	}

	ClientComponent::ClientComponent(std::string ip, unsigned int port)
	{
		socket.connect(CONNECTION_TIMEOUT, ip, port);
		messageDispatcher = eventListenerBind(&ClientComponent::dispatchEvent, this);
		socket.onRecive.subscribe(&messageDispatcher);
	}
}