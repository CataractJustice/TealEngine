#include "GameNode.h"
#include <unordered_set>
#include "EventSystem/GameNodeEvents/GameNodeEvents.h"
#include "Component.h"
#include "libs/imgui/imgui.h"

using namespace std;
namespace TealEngine {

	unordered_set<GameNode*> GameNode::allNodes = unordered_set<GameNode*>(8192);
	map<string, set<GameNode*>> GameNode::tagged = map<string, set<GameNode*>>();

	//sets parrent of a node, calls GameNode::onParrentChange after setting new parrent
	void GameNode::setParrent(GameNode* parrent)
	{
		if (this->parrent)
			this->parrent->removeChild(this);
		this->parrent = parrent;
		this->hierarchyDepth = parrent->getHierarchyDepth() + 1;
		onParrentChange();
	}

	const std::string& GameNode::getName() { return name; }
	void GameNode::rename(const string& name) { this->name = name; }

	//tag system methods

	//checks if node has given tag added
	bool GameNode::hasTag(const string& tag)
	{
		return tags.find(tag) != tags.cend();
	}


	void GameNode::addTag(const string& tag)
	{
		if (!hasTag(tag))
		{
			this->tags.insert(tag);
			if (tagged.find(tag) == tagged.end())
				tagged[tag] = set<GameNode*>();
			tagged[tag].insert(this);
		}
	}

	void GameNode::removeTag(const string& tag)
	{
		if (hasTag(tag)) {
			this->tags.erase(tag);
			tagged[tag].erase(this);
		}
	}

	set<GameNode*> GameNode::findNodesByTag(const string& tag)
	{
		return tagged[tag];
	}
	//components:

	void GameNode::attachComponent(Component* component) 
	{
		this->components.push_back(component);
		component->attachTo(this);
	}
	void GameNode::attachComponents(const std::initializer_list<Component*>& components)
	{
		for (auto& component : components)
			attachComponent(component);
	}
	void GameNode::dettachComponent(Component* component) 
	{
		if (component->getParrent() == this) 
		{
			components.remove(component);
			component->parrent = nullptr;
		}
		else
		{
			throw "tried to remove component from a node to whitch it does not belong";
		}
	}

	unsigned short GameNode::getHierarchyDepth()
	{
		return hierarchyDepth;
	}

	GameNode* GameNode::addChild(GameNode* node)
	{
		node->setParrent(this);
		this->childNodes.push_back(node);
		ChildNodeAddEvent e = ChildNodeAddEvent(node);
		handleEvent(&e);
		return childNodes.back();
	}

	void GameNode::addChilds(const std::initializer_list<GameNode*>& nodes)
	{
		for (auto& node : nodes) this->addChild(node);
	}

	void GameNode::removeChild(GameNode* node)
	{
		if (node->getParrent() == this) 
		{
			this->childNodes.remove(node);
			ChildNodeRemoveEvent e = ChildNodeRemoveEvent(node);
			handleEvent(&e);
			node->parrent = nullptr;
		}
		else
		{
			throw "tried to remove child from a node to whitch it does not belong";
		}
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
	bool GameNode::getActive() { return active && (getParrent() ? getParrent()->getActive() : true); }
	//events:
	void GameNode::setActive(bool active) 
	{
		if(active && !this->active)
			for (Component* comp : components)
				if (comp->getActive())
					comp->onAwake();
		else
			if(!active && this->active)
				for (Component* comp : components)
					if (comp->getActive())
						comp->onSleep();
		this->active = active;
	}
	void GameNode::onParrentChange() 
	{
		for (Component* comp : components)
			comp->onParrentChange();
	};
	void GameNode::update()
	{
		if (!active) return;
		for (Component* comp : components)
			if(comp->getActive())
				comp->update();
	};

	void GameNode::updateAll()
	{
		if (!active) return;
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

	void GameNode::subNodeToEvent(EventType type, GameNode* node) 
	{
		this->eventSubscribedNodes[type].push_back(node);
	}

	EventListener* GameNode::getEventListener(EventType type)
	{
		return (this->eventListeners.find(type) == eventListeners.end()) ? nullptr : &eventListeners[type];
	}

	void GameNode::handleEvent(Event* e, bool toLower, bool toUpper)
	{
		if (eventListeners.find(e->getType()) != eventListeners.end())
			this->eventListeners[e->getType()](e);
		for (GameNode* node : eventSubscribedNodes[e->getType()]) 
		{
			node->handleEvent(e, false, false);
		}
		
		if (toLower) 
		{
			for (GameNode* child : childNodes)
				child->handleEvent(e, true);
		}
		
		if (this->parrent && toUpper) this->parrent->handleEvent(e);
	}

	//Network:

	void GameNode::onMessageReceive() {
		if (!active) return;
		for (Component* component : components) {
			if (component->getActive())
				component->onMessageReceive();
		}
	}

	GameNode::GameNode()
	{
		willBeDestroyed = false;
		childNodes = list<GameNode*>(0);
		components = list<Component*>(0);
		parrent = NULL;
		hierarchyDepth = 0;
		allNodes.insert(this);
		active = true;
		id = lastId;
		lastId++;
	}

	GameNode::~GameNode()
	{
		//removing node from parrent child nodes
		if (parrent) parrent->removeChild(this);
		if(!onDestroyHasBeenCalled)
			if(components.size())
				for (Component* comp : components)
					if (comp->getActive())
						comp->onDestroy();
		onDestroyHasBeenCalled = true;
		//recursive delete all the child nodes
		//using temporal list of nodes cuz node destructor will modify childNodes list
		list<GameNode*> nodes = childNodes;
		if(childNodes.size())
			for (GameNode* node : nodes)
			{
				delete node;
			}
		std::list<Component*> comps = components;

		if (components.size())
			for (Component* comp : comps)
				delete comp;

		allNodes.erase(this);
	}

	bool GameNode::isNodeExist(GameNode* node)
	{
		return allNodes.find(node) != allNodes.end();
	}

	bool GameNode::isVisibleForPeer() 
	{
		return true;
	}

	void GameNode::onCollision(const Physics::Collision& collision, bool eventDown, bool eventUp) 
	{
		if (!active) return;
		for (Component* comp : components) 
		{
			if (comp->getActive())
				comp->onCollision(collision);
		}

		if(eventUp && this->parrent)
			this->parrent->onCollision(collision, false);

		if(eventDown) 
		{
			for(GameNode* node : childNodes) 
			{
				node->onCollision(collision, true, false);
			}
		}
	}

	void GameNode::GUIrender()
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->GUIrender();
		}

		for (GameNode* node : childNodes)
		{
			node->GUIrender();
		}
	}

	void GameNode::imGUIrender()
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->imGUIrender();
		}

		for (GameNode* node : childNodes)
		{
			node->imGUIrender();
		}
	}

	void GameNode::render(ShaderProgram* shader, unsigned int stages)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->render(shader, stages);
		}

		for (GameNode* node : childNodes)
		{
			node->render(shader, stages);
		}
	}

	void GameNode::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->postProcess(unlitColor, litColor, position, normal, specular, light);
		}

		for (GameNode* node : childNodes)
		{
			node->postProcess(unlitColor, litColor, position, normal, specular, light);
		}
	}

	void GameNode::cleanUp() 
	{
		while(destroyQueue.size()) 
		{
			delete destroyQueue.front();
			destroyQueue.pop();
		}
	}


	void GameNode::displayNodeTree(bool windowBegin) 
	{
		if(windowBegin) 
		{
			ImGui::Begin("Node tree");
		}
		if(ImGui::TreeNode(((this->name.length() ? this->name : std::string("unnamed")) + " [" + std::to_string(id) + "]").c_str()))
		{
			ImGui::Checkbox("Active", &active);
			if (ImGui::TreeNode("Components")) 
			{
				for (Component* comp : components)
				{
					comp->explorerDisplay();
				}
				ImGui::TreePop();
			}
			for(GameNode* node : this->childNodes) 
			{
				node->displayNodeTree(false);
			}
			ImGui::TreePop();
		}
		if(windowBegin) 
		{
			ImGui::End();
		}
	}

	std::queue<GameNode*> GameNode::destroyQueue;
	unsigned int GameNode::lastId = 0;
}