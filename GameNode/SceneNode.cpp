#include "SceneNode.h"
#include "System/VTable.h"
#include "Component.h"
namespace TealEngine 
{
	void SceneNode::onChildAdd(GameNode* child, GameNode* parent) 
	{
		child->setSceneNode(this);
		fullSceneNodeList.emplace(child);
		for(Component* c : child->getComponents()) 
		{
			onComponentAdd(c, child);
		}
		for(GameNode* node : child->getChilds()) 
		{
			onChildAdd(node, child);
		}
		for(Component* c : onSceneChildAddCallbackList) 
		{
			c->onSceneChildAdd(child, parent);
		}
	}

	void SceneNode::onChildRemove(GameNode* child, GameNode* parent) 
	{
		fullSceneNodeList.erase(child);
		for(Component* c : child->getComponents()) 
		{
			onComponentRemove(c, child);
		}
		for(GameNode* node : child->getChilds()) 
		{
			onChildRemove(node, child);
		}
		for(Component* c : onSceneChildRemoveCallbackList) 
		{
			c->onSceneChildRemove(child, parent);
		}
	}


	//
	void SceneNode::onComponentAdd(Component* component, GameNode* node) 
	{
		fullSceneComponentList.emplace(component);
		addComponentToCallbackLists(component);
		updateComponentNetworkId({0,0}, component->getNetworkId(), component);
		for(Component* c : onSceneComponentAddCallbackList) 
		{
			c->onSceneComponentAdd(component, node);
		}
	}
	//
	void SceneNode::onComponentRemove(Component* component, GameNode* node) 
	{
		fullSceneComponentList.erase(component);
		removeComponentFromCallbackLists(component);
		for(Component* c : onSceneComponentRemoveCallbackList) 
		{
			c->onSceneComponentRemove(component, node);
		}
		{
			auto it = networkIdComponentsMap.find(component->getNetworkId()[0]);
			if(it != networkIdComponentsMap.cend()) 
			{
				auto it2 = it->second.find(component->getNetworkId()[1]);
				if(it2 != it->second.cend()) 
				{
					it2->second.erase(component);
					if(it2->second.size() == 0) 
					{
						it->second.erase(it2);
					}
				}
				if(it->second.size() == 0) 
				{
					networkIdComponentsMap.erase(it);
				}
			}
		}
		return;
	}

	void SceneNode::update() 
	{
		for(Component* c : fullSceneComponentList) 
		{
			if(!c->isOnAttachCallbackHasBeenCalled()) 
			{
				c->onAttach();
				c->cancelOnAttachCallback();
			}
		}
		for(Component* c : updateCallbackList) 
		{
			if(!c->isOnAttachCallbackHasBeenCalled()) 
			{
				c->onAttach();
				c->cancelOnAttachCallback();
			}
			c->update();
		}
	}

	void SceneNode::editorUpdate() 
	{
		for(Component* c : editorUpdateCallbackList) 
		{
			c->editorUpdate();
		}
	}

	void SceneNode::windowResize(int width, int height) 
	{
		for(Component* c : onWindowResizeCallbackList) 
		{
			c->onWindowResize(width, height);
		}
	}

	void SceneNode::onHostPeerConnect(PeerInfo& peerInfo) 
	{
		for(Component* c : onHostPeerConnectCallbackList) 
		{
			c->onHostPeerConnect(peerInfo);
		}
	};
	void SceneNode::onHostPeerDisconnect(PeerInfo& peerInfo) 
	{
		for(Component* c : onHostPeerConnectCallbackList) 
		{
			c->onHostPeerDisconnect(peerInfo);
		}
	};
	void SceneNode::onHostPeerPacket(const NetworkId& componentId, PeerInfo& peerInfo, const void* data, int size) 
	{
		auto it = this->networkIdComponentsMap.find(componentId[0]);
		if(it == networkIdComponentsMap.cend()) return;
		auto it2 = it->second.find(componentId[1]);
		if(it2 == it->second.cend()) return;
		for(Component* c : it2->second) 
		{
			c->onHostPeerPacket(peerInfo, data, size);
		}
	};
	void SceneNode::onClientConnect() 
	{
		for(Component* c : onClientConnectCallbackList) 
		{
			c->onClientConnect();
		}
	};
	void SceneNode::onClientConnectFail() 
	{
		for(Component* c : onClientConnectFailCallbackList) 
		{
			c->onClientConnectFail();
		}
	};
	void SceneNode::onClientDisconnect() 
	{
		for(Component* c : onClientDisconnectCallbackList) 
		{
			c->onClientDisconnect();
		}
	};
	void SceneNode::onClientPacket(const NetworkId& componentId, const void* data, int size) 
	{
		auto it = this->networkIdComponentsMap.find(componentId[0]);
		if(it == networkIdComponentsMap.cend()) return;
		auto it2 = it->second.find(componentId[1]);
		if(it2 == it->second.cend()) return;
		for(Component* c : it2->second) 
		{
			c->onClientPacket(data, size);
		}
	};

	void SceneNode::onKeyPress(unsigned int key) 
	{
		for(Component* c : this->onKeyPressCallbackList) 
		{
			c->onKeyPress(key);
		}
	}
	void SceneNode::onKeyRelease(unsigned int key) 
	{
		for(Component* c : this->onKeyReleaseCallbackList) 
		{
			c->onKeyRelease(key);
		}
	}
	void SceneNode::onKeyRepeat(unsigned int key) 
	{
		for(Component* c : this->onKeyRepeatCallbackList) 
		{
			c->onKeyRepeat(key);
		}
	}
	void SceneNode::onChar(unsigned int key) 
	{
		for(Component* c : this->onCharCallbackList) 
		{
			c->onChar(key);
		}
	}

	__attribute__((optimize("O0"))) void SceneNode::addComponentToCallbackLists(Component* c) 
	{
		if(isMethodOverriden(Component::base, c, &Component::update)) 
		{
			this->updateCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneChildAdd)) 
		{
			this->onSceneChildAddCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneChildRemove)) 
		{
			this->onSceneChildRemoveCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneComponentAdd)) 
		{
			this->onSceneComponentAddCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneComponentRemove)) 
		{
			this->onSceneComponentRemoveCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::editorUpdate)) 
		{
			this->editorUpdateCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onWindowResize)) 
		{
			this->onWindowResizeCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onHostPeerConnect)) 
		{
			this->onHostPeerConnectCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onHostPeerDisconnect)) 
		{
			this->onHostPeerDisconnectCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientConnect)) 
		{
			this->onClientConnectCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientConnectFail)) 
		{
			this->onClientConnectFailCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientDisconnect)) 
		{
			this->onClientDisconnectCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onKeyPress)) 
		{
			this->onKeyPressCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onKeyRelease)) 
		{
			this->onKeyReleaseCallbackList.emplace(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onChar)) 
		{
			this->onCharCallbackList.emplace(c);
		}
	}
	void SceneNode::removeComponentFromCallbackLists(Component* c) 
	{
		if(isMethodOverriden(Component::base, c, &Component::update)) 
		{
			this->updateCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneChildAdd)) 
		{
			this->onSceneChildAddCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneChildRemove)) 
		{
			this->onSceneChildRemoveCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneComponentAdd)) 
		{
			this->onSceneComponentAddCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onSceneComponentRemove)) 
		{
			this->onSceneComponentRemoveCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::editorUpdate)) 
		{
			this->editorUpdateCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onWindowResize)) 
		{
			this->onWindowResizeCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onHostPeerConnect)) 
		{
			this->onHostPeerConnectCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onHostPeerDisconnect)) 
		{
			this->onHostPeerDisconnectCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientConnect)) 
		{
			this->onClientConnectCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientConnectFail)) 
		{
			this->onClientConnectFailCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onClientDisconnect)) 
		{
			this->onClientDisconnectCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onKeyPress)) 
		{
			this->onKeyPressCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onKeyRelease)) 
		{
			this->onKeyReleaseCallbackList.erase(c);
		}
		if(isMethodOverriden(Component::base, c, &Component::onChar)) 
		{
			this->onCharCallbackList.erase(c);
		}
	}

	void SceneNode::updateComponentNetworkId(const NetworkId& oldId, const NetworkId& newId, Component* component) 
	{
		networkIdComponentsMap[newId[0]][newId[1]].emplace(component);
		auto oldIt = networkIdComponentsMap.find(oldId[0]);
		if(oldIt == networkIdComponentsMap.cend()) return;
		auto oldIt2 = oldIt->second.find(oldId[1]);
		if(oldIt2 == oldIt->second.cend()) return;
		oldIt2->second.erase(component);
		if(oldIt2->second.size() == 0) oldIt->second.erase(oldIt2);
		if(oldIt->second.size() == 0) networkIdComponentsMap.erase(oldIt);
	}
}