#pragma once
#include "GameNode.h"
#include <unordered_set>
#include <map>
namespace TealEngine 
{
	class SceneNode : public GameNode
	{
		private:
		std::unordered_set<GameNode*> fullSceneNodeList;
		std::unordered_set<Component*> fullSceneComponentList;
		std::unordered_set<Component*> updateCallbackList;
		std::unordered_set<Component*> editorUpdateCallbackList;
		std::unordered_set<Component*> onSceneChildAddCallbackList;
		std::unordered_set<Component*> onSceneComponentAddCallbackList;
		std::unordered_set<Component*> onSceneChildRemoveCallbackList;
		std::unordered_set<Component*> onSceneComponentRemoveCallbackList;
		std::unordered_set<Component*> onWindowResizeCallbackList;
		std::unordered_set<Component*> onHostPeerConnectCallbackList;
		std::unordered_set<Component*> onHostPeerDisconnectCallbackList;
		std::unordered_set<Component*> onClientConnectCallbackList;
		std::unordered_set<Component*> onClientConnectFailCallbackList;
		std::unordered_set<Component*> onClientDisconnectCallbackList;
		std::unordered_set<Component*> onKeyPressCallbackList;
		std::unordered_set<Component*> onKeyReleaseCallbackList;
		std::unordered_set<Component*> onKeyRepeatCallbackList;
		std::unordered_set<Component*> onCharCallbackList;
		std::map<unsigned long long, std::map<unsigned long long, std::unordered_set<Component*>>> networkIdComponentsMap;
		void addComponentToCallbackLists(Component* c);
		void removeComponentFromCallbackLists(Component* c);
		public:
		//
		virtual void onChildAdd(GameNode* child, GameNode* parent) override;
		//
		virtual void onChildRemove(GameNode* child, GameNode* parent) override;
		//
		virtual void onComponentAdd(Component* component, GameNode* node) override;
		//
		virtual void onComponentRemove(Component* component, GameNode* node) override;
		//
		virtual void update() override;
		//
		virtual void editorUpdate() override;
		//
		virtual void windowResize(int width, int height);
		/*
		Network section
		*/
		//called on a host every time a new client is connected
		virtual void onHostPeerConnect(PeerInfo& peerInfo);
		//called on a host every time a client disconnects
		virtual void onHostPeerDisconnect(PeerInfo& peerInfo);
		//called on a host every time a packet is received from a client
		virtual void onHostPeerPacket(const NetworkId& componentId, PeerInfo& peerInfo, const void* data, int size);
		//called on a client after connecting to a host
		virtual void onClientConnect();
		//called on a client after fail connecting to a host
		virtual void onClientConnectFail();
		//called on a client after disconnecting from a host
		virtual void onClientDisconnect();
		//called on a client after receiving a packet from a host
		virtual void onClientPacket(const NetworkId& componentId, const void* data, int size);
		//
		virtual void onKeyPress(unsigned int key);
		//
		virtual void onKeyRelease(unsigned int key);
		//
		virtual void onKeyRepeat(unsigned int key);
		//
		virtual void onChar(unsigned int unicode);

		void updateComponentNetworkId(const NetworkId& oldId, const NetworkId& newId, Component* component);
	};
}