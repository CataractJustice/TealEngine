#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include "EventSystem/Event.h"
#include "EventSystem/EventListener.h"
#include "System/Debug.h"
#include "Physics/Collision.h"
#include <queue>
#include <filesystem>
#include "NlohmannJson/json.hpp"

using Json = nlohmann::json;

enum NODE_RELATION 
{
	THIS_NODE,
	NOT_RELATED_NODE,
	PARENT_NODE,
	CHILD_NODE
};

namespace TealEngine {
	class ShaderProgram;
	class Component;
	class GameNode
	{
	private:
		static unordered_set<GameNode*> allNodes;
		static std::map<int, GameNode*> idMap;
		static map<string, set<GameNode*>> tagged;
		static std::queue<GameNode*> destroyQueue;
		
		map<EventType, EventListener> eventListeners;
		map<EventType, std::list<GameNode*>> eventSubscribedNodes;
		set<string> tags;
		bool willBeDestroyed;
		unsigned short hierarchyDepth;
		static unsigned int lastId;
		static GameNode* selectedNode;
	protected:
		bool active;
		unsigned int id;
		bool onDestroyHasBeenCalled;
		string name;
		list<GameNode*> childNodes;
		list<Component*> components;
		GameNode* parent;
		std::string loadedFromFilePath;

		virtual void setParent(GameNode* parent);
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

		//returns parent node, returns nullptr if node is an orphan
		GameNode* getParent();

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

		//gets parent if parent is of type T, else throws an error
		template<typename T>
		T* getParentOfType() 
		{
			T* castedParent = dynamic_cast<T*>(parent);
			if (!castedParent)
				TE_DEBUG_ERROR("Parent either does not exist or isnt the right type.");
			return castedParent;
		}
		//returns all child nodes
		std::vector<GameNode*> getChilds();
		//returns childs, childs of childs an so on in a single vector
		std::vector<GameNode*> getAllChilds();
		//returns childs with matching name
		std::vector<GameNode*> findChildsByName(std::string name);
		//node is active only if active property is true for it and all of its parents
		//use .getActiveProperty to get this exact node activity property
		bool getActive();
		//returns value of node activity property
		//node can be innactive while having activity property set to true but any of it's parent nodes is innactive
		//use .getActive() to check if node is actually active
		inline bool getActiveProperty() { return this->active; };
		//events:

		//enables/disables node, calls onSleep or onAwake for components
		void setActive(bool active);
		//called when parent changes
		virtual void onParentChange();
		//usualy called every frame
		virtual void update() final;
		//recursively updates current and all lower nodes
		void updateAll();
		void editorUpdate();
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

		//
		virtual void onCollision(const Physics::Collision& collision, bool eventDown = true, bool eventUp = true);
		//
		void GUIrender();
		//
		void imGuiRender(const std::string& windowName);
		//
		void render(ShaderProgram* shader = nullptr, unsigned int stages = 0);
		void renderId();

		//
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light);

		virtual void displayNodeTree();

		virtual void displayProps();

		//deletes node when it's possible
		inline void destroy() { if (!willBeDestroyed) { destroyQueue.push(this); willBeDestroyed = true; } };

		static void cleanUp();

		GameNode();
		virtual ~GameNode();
		static bool isNodeExist(GameNode* node);

		virtual Json toJson();

		int getId();
		void setId(int id);
		static GameNode* getNodeById(int id);
		static GameNode* getSelectedNode();
		static void setSelectedNode(GameNode* node);

		void setNodeFilePath(std::filesystem::path path);

		void save();
		void saveAs(const std::filesystem::path& path);
	};
}