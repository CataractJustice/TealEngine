#pragma once
#include "GameNode.h"
#include <unordered_set>
#include "EventSystem/GameNodeEvents/GameNodeEvents.h"
#include "Network/Server.h"

using namespace std;
namespace TealEngine {

	unordered_set<GameNode*> allNodes;
	map<string, set<GameNode*>> tagged;

	void GameNode::init() 
	{
		allNodes = unordered_set<GameNode*>();
		tagged = map<string, set<GameNode*>>();
	}

	void GameNode::setParrent(GameNode* parrent)
	{
		if (this->parrent)
			this->parrent->removeChild(this);
		this->parrent = parrent;
		this->hierarchyDepth = parrent->getHierarchyDepth() + 1;
	}

	string GameNode::getName() { return (name.length() == 0) ? "unnamed" : name; }

	void GameNode::rename(string name) { this->name = name; }

	//tag system methods
	bool GameNode::hasTag(string tag)
	{
		return tags.find(tag) != tags.end();
	}

	void GameNode::addTag(string tag)
	{
		if (!hasTag(tag))
		{
			this->tags.insert(tag);
			if (tagged.find(tag) == tagged.end())
				tagged[tag] = set<GameNode*>();
			tagged[tag].insert(this);
		}
	}

	void GameNode::removeTag(string tag)
	{
		if (hasTag(tag)) {
			this->tags.erase(tag);
			tagged[tag].erase(this);
		}
	}

	set<GameNode*> GameNode::findNodesByTag(string tag)
	{
		return tagged[tag];
	}
	//

	unsigned short GameNode::getHierarchyDepth()
	{
		return hierarchyDepth;
	}

	NODE_RELATION GameNode::checkRelation(GameNode* node)
	{
		//if nodes appears to be on the same layer of hierarchy then its not related nodes or the same node
		if (node->getHierarchyDepth() == getHierarchyDepth())
		{
			if (node == this)
				return THIS_NODE;
			else
				return NOT_RELATED_NODE;
		}//if check
		else if (node->getHierarchyDepth() < getHierarchyDepth())
		{
			if (getParrent() == nullptr || getHierarchyDepth() == 0)
				return NOT_RELATED_NODE;

			if (getParrent() == node)
				return PARRENT_NODE;

			return getParrent()->checkRelation(node);
		}
		else
		{
			return (node->checkRelation(this) == PARRENT_NODE) ? CHILD_NODE : NOT_RELATED_NODE;
		}
	}

	GameNode* GameNode::addChild(GameNode* node)
	{
		node->setParrent(this);
		this->childNodes.push_back(node);
		handleEvent(&ChildNodeAddEvent(node));
		return childNodes.back();
	}

	void GameNode::addChilds(vector<GameNode*> nodes)
	{
		for (unsigned int i = 0; i < nodes.size(); i++) this->addChild(nodes[i]);
	}

	void GameNode::removeChild(GameNode* node)
	{
		this->childNodes.remove(node);
		handleEvent(&ChildNodeRemoveEvent(node));
		node->parrent = nullptr;
	}

	std::vector<GameNode*> GameNode::getChilds()
	{
		std::vector<GameNode*> n;
		for (GameNode* node : childNodes)
		{
			n.push_back(node);
		}
		return n;
	}

	std::vector<GameNode*> GameNode::getAllChilds()
	{
		std::vector<GameNode*> n;
		for (GameNode* node : childNodes)
		{
			n.push_back(node);
			std::vector<GameNode*> childChildNodes = node->getChilds();
			n.insert(n.end(), childChildNodes.begin(), childChildNodes.end());
		}
		return n;
	}

	std::vector<GameNode*> GameNode::findChildsByName(std::string name)
	{
		std::vector<GameNode*> res;
		for (GameNode* node : childNodes)
		{
			if (node->name == name)
			{
				res.push_back(node);
			}
		}
		return res;
	}

	GameNode* GameNode::getParrent()
	{
		return parrent;
	}

	//events:
	void GameNode::update() {};

	void GameNode::updateAll()
	{
		//recursive update all the child nodes
		for (GameNode* node : childNodes)
		{
			node->updateAll();
		}
		//update this node
		this->update();
	}

	void GameNode::addEventListener(EventType type, eventListenerFunc ex)
	{
		this->eventListeners[type] = ex;
	}

	EventListener* GameNode::getEventListener(EventType type)
	{
		return (this->eventListeners.find(type) == eventListeners.end()) ? nullptr : &eventListeners[type];
	}

	void GameNode::handleEvent(Event* e, bool toLower)
	{
		if (eventListeners.find(e->getType()) != eventListeners.end())
			this->eventListeners[e->getType()](e);
		
		if (toLower) 
		{
			for (GameNode* child : childNodes)
				child->handleEvent(e, true);
		}
		else
		if (this->parrent) this->parrent->handleEvent(e);
	}

	GameNode::GameNode()
	{
		childNodes = list<GameNode*>(0);
		parrent = NULL;
		hierarchyDepth = 0;
		allNodes.insert(this);
	}

	GameNode::~GameNode()
	{
		//recursive delete all the child nodes
		//using temporal list of nodes cuz node destructor will modify childNodes list
		list<GameNode*> nodes = childNodes;
		if(childNodes.size() > 0)
			for (GameNode* node : nodes)
			{
				delete node;
			}

		//removing node from parrent child nodes
		if (parrent) parrent->removeChild(this);

		allNodes.erase(this);
	}

	bool GameNode::isNodeExist(GameNode* node)
	{
		return allNodes.find(node) != allNodes.end();
	}
}