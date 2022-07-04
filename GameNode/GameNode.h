#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include "EventSystem/Event.h"
#include "EventSystem/EventListener.h"
#include "System/Debug.h"
using namespace std;
enum NODE_RELATION 
{
	THIS_NODE,
	NOT_RELATED_NODE,
	PARRENT_NODE,
	CHILD_NODE
};

namespace TealEngine {

class Component;
	class GameNode
	{
	private:
		static unordered_set<GameNode*> allNodes;
		static map<string, set<GameNode*>> tagged;
		
		map<EventType, EventListener> eventListeners;
		map<EventType, std::list<GameNode*>> eventSubscribedNodes;
		set<string> tags;
		bool active;
		unsigned short hierarchyDepth;
	protected:
		string name;
		list<GameNode*> childNodes;
		list<Component*> components;
		GameNode* parrent;

		virtual void setParrent(GameNode* parrent);
	public:
		const string& getName();
		void rename(const string& name);

		//tag system

		//checks if node has given tag added
		bool hasTag(const string& tag);
		//adds tag to a node
		void addTag(const string& tag);
		//removes tag from node
		void removeTag(const string& tag);
		//returns set of ALL nodes with given tag added
		static set<GameNode*> findNodesByTag(const string& tag);

		//component system

		//detaches component if it's attached to anything and attaches it to this node
		void attachComponent(Component* component);
		//detaches components if they attached to anything and attaches them to this node
		void attachComponents(const std::initializer_list<Component*>& component);
		//detaches component if component is attachet to this node, else throws an error
		void dettachComponent(Component* component);
		template<class T>
		//returns all components of given type
		std::vector<T*> findComponentsByType() 
		{
			std::vector<T*> comps;
			for (Component* component : components)
				if (dynamic_cast<T*>(component))
					comps.push_back((T*)component);
			return comps;
		}


		//returns length of path from root node to this node
		unsigned short getHierarchyDepth();

		[[deprecated]]
		NODE_RELATION checkRelation(GameNode* node);
		
		//returns parrent node, returns nullptr if node is an orphan
		GameNode* getParrent();

		//adds child nodes and calls handleEvent with CHILD_ADDED event
		virtual GameNode* addChild(GameNode* node);
		//calls addChild in a loop for every vector element
		virtual void addChilds(const std::initializer_list<GameNode*>& nodes);
		//removes given child if it is a child of this node, else throws an error
		virtual void removeChild(GameNode* node);

		//returns every child node of given type
		template<typename T>
		std::vector<T*> findChildsByType(bool childsOfChilds = false) 
		{
			std::vector<T*> res;
			for (GameNode* node : childNodes)
			{
				if (dynamic_cast<T*>(node))
					res.push_back((T*)node);

				if (childsOfChilds)
				{
					vector<T*> childsOfChild = node->findChildsByType<T>(true);
					if (childsOfChild.size() > 0)
						res.insert(res.end(), childsOfChild.begin(), childsOfChild.end());
				}
			}
			return res;
		}

		//gets parrent if parrent is of type T, else throws an error
		template<typename T>
		T* getParrentOfType() 
		{
			T* castedParrent = dynamic_cast<T*>(parrent);
			if (!castedParrent)
				TE_DEBUG_ERROR("Parrent either does not exist or isnt the right type.");
			return castedParrent;
		}
		//returns all child nodes
		std::vector<GameNode*> getChilds();
		//returns childs, childs of childs an so on in a single vector
		std::vector<GameNode*> getAllChilds();
		//returns childs with matching name
		std::vector<GameNode*> findChildsByName(std::string name);
		//node is active only if active property is true for it and all of its parrents
		//use .getActiveProperty to get this exact node activity property
		bool getActive();
		//returns value of node activity property
		//node can be innactive while having activity property set to true but any of it's parrent nodes is innactive
		//use .getActive() to check if node is actually active
		inline bool getActiveProperty() { return this->active; };
		//events:

		//enables/disables node, calls onSleep or onAwake for components
		void setActive(bool active);
		//called when parrent changes
		virtual void onParrentChange();
		//usualy called every frame
		virtual void update() final;
		//recursively updates current and all lower nodes
		void updateAll();
		//adds function that will be called on call of handleEvent with event of given type
		void addEventListener(EventType type, eventListenerFunc ex);
		//handleEvent will be called on param node when handleEvent on this node will be called with event of given type 
		void subNodeToEvent(EventType type, GameNode* node);
		//a?
		EventListener* getEventListener(EventType type);
		//sends event to added event listeners
		void handleEvent(Event* e, bool toLower = false, bool toUpper = true);

		//Network:
		
		//Called whenever node recives a messgae, calls onMessageReceive(packet) for all components of this node
		virtual void onMessageReceive();
		//Server side method
		//Should return true if you want to node to be 'visible' for given peer
		//Server will send node itself and messages to node to given peer if node is 'visible' for them
		//Should return false if server should not send any information about this node to given peer
		//Returned value for certain peer might change during lifetime of the node
		virtual bool isVisibleForPeer();

		GameNode();
		virtual ~GameNode();
		static bool isNodeExist(GameNode* node);
	};
}