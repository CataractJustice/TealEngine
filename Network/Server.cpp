#include "Server.h"
#include "PacketTemplates.h"
#include "GameNode/Component.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine
{
	void Server::onConnect(Event* e) 
	{
		PeerConnectEvent* pce = (PeerConnectEvent*)e;
	}

	NodeNetworkIDType Server::getNodeId(GameNode* node)
	{
		return (NodeNetworkIDType)node;
	}

	ComponentNetworkIDType Server::getComponentId(Component* component) 
	{
		return (ComponentNetworkIDType)component;
	}

	void Server::start(int port, int connections)
	{
		host.host(port, connections);
	}

	void Server::update()
	{
	/*	if ((rand() % 1000000) == 0)
		{
			GameNode* n = new GameNode();
			addNodeToNetwork(n);
			Component* c = ComponentFactory::instantiateComponent("NetworkTest");
			n->attachComponent(c);
			addComonentToNetwork(c);
			TPacket nodepacket;
			nodepacket.push("Message to node.");

			TPacket componentpacket;
			componentpacket.push("Message to component.");
			broadcastToNode(n, nodepacket);
			broadcastToComponent(c, componentpacket);
		}
		*/
		host.pollEvents();
	}

	void Server::broadcastToNode(GameNode* node, TPacket& pakcet)
	{
		NodeNetworkIDType id = getNodeId(node);
		TPacket nodePacket;

		nodePacket.pushReserve(PacketTemplates::nodePacketTemplate.getLength());
		nodePacket.setFieldValue(id, PacketTemplates::nodePacketTemplate.getIndexOf("node_id"));

		unsigned int packetSize = 0;
		uint8_t* packetStr = pakcet.constructDataString(packetSize);
		nodePacket.setFieldValue(packetSize, packetStr, PacketTemplates::nodePacketTemplate.getIndexOf("packet"));

		TPacket finalPacket;
		finalPacket.push(ServerPacketEType::BroadcastToNode);
		finalPacket += nodePacket;

		std::map<unsigned int, PeerData> peers = host.getPeers();

		for (auto peerpair : peers) 
		{
			if (node->isVisibleForPeer(peerpair.second)) 
			{
				host.send(finalPacket, peerpair.second);
			}
		}
	}

	void Server::broadcastToComponent(Component* component, TPacket& pakcet) {
		ComponentNetworkIDType id = getComponentId(component);
		TPacket compPacket;

		compPacket.pushReserve(PacketTemplates::componentPacketTemplate.getLength());
		compPacket.setFieldValue(id, PacketTemplates::componentPacketTemplate.getIndexOf("component_id"));

		unsigned int packetSize = 0;
		uint8_t* packetStr = pakcet.constructDataString(packetSize);
		compPacket.setFieldValue(packetSize, packetStr, PacketTemplates::nodePacketTemplate.getIndexOf("packet"));

		TPacket finalPacket;
		finalPacket.push(ServerPacketEType::BroadcastToComponent);
		finalPacket += compPacket;

		std::map<unsigned int, PeerData> peers = host.getPeers();

		for (auto peerpair : peers) 
		{
			if (component->getParrent()->isVisibleForPeer(peerpair.second)) 
			{
				host.send(finalPacket, peerpair.second);
			}
		}

		delete[] packetStr;
	}

	void Server::addNodeToNetwork(GameNode* node) 
	{
		TPacket packet;
		packet.pushReserve(PacketTemplates::newNodePacketTemplate.getLength());
		packet.setFieldValue(getNodeId(node), PacketTemplates::newNodePacketTemplate.getIndexOf("node_id"));

		TPacket packetPrefix;
		packetPrefix.push(ServerPacketEType::NewNode);

		std::map<unsigned int, PeerData> peers = host.getPeers();

		for (auto peerpair : peers) 
		{
			if (node->isVisibleForPeer(peerpair.second)) 
			{
				host.send(packetPrefix + packet, peerpair.second);
			}
		}
	}

	void Server::removeNodeFromNetwork(GameNode* node) 
	{
		TPacket packet;
		packet.pushReserve(PacketTemplates::newNodePacketTemplate.getLength());
		packet.setFieldValue(getNodeId(node), PacketTemplates::newNodePacketTemplate.getIndexOf("node_id"));

		TPacket packetPrefix;
		packetPrefix.push(ServerPacketEType::RemoveNode);

		std::map<unsigned int, PeerData> peers = host.getPeers();

		host.broadcast(packetPrefix + packet);
	}

	void Server::addComonentToNetwork(Component* component)
	{
		GameNode* attachTo = component->getParrent();

		if (!attachTo) 
		{
			TE_DEBUG_ERROR("Attach component to anything before adding it to network.");
		}

		TPacket packet;
		packet.pushReserve(PacketTemplates::newComponentPacketTemplate.getLength());

		std::string ComponentFactoryName = component->getFactoryName();
		packet.setFieldValue(ComponentFactoryName.size(), (void*)ComponentFactoryName.c_str(), PacketTemplates::newComponentPacketTemplate.getIndexOf("component_type"));


		packet.setFieldValue(getComponentId(component), PacketTemplates::newComponentPacketTemplate.getIndexOf("component_id"));
		packet.setFieldValue(getNodeId(attachTo), PacketTemplates::newComponentPacketTemplate.getIndexOf("node_id"));

		TPacket packetPrefix;
		packetPrefix.push(ServerPacketEType::NewComponent);

		std::map<unsigned int, PeerData> peers = host.getPeers();

		for (auto peerpair : peers)
		{
			if (attachTo->isVisibleForPeer(peerpair.second))
			{
				host.send(packetPrefix + packet, peerpair.second);
			}
		}
	}

	void Server::removeComponentFromNetwork(Component* component)
	{
		TPacket packet;
		packet.pushReserve(PacketTemplates::newNodePacketTemplate.getLength());
		packet.setFieldValue(getComponentId(component), PacketTemplates::newNodePacketTemplate.getIndexOf("node_id"));

		TPacket packetPrefix;
		packetPrefix.push(ServerPacketEType::RemoveComponent);

		std::map<unsigned int, PeerData> peers = host.getPeers();

		host.broadcast(packetPrefix + packet);
	}
}