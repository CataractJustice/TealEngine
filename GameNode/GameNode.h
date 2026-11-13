#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include "System/Debug.h"
#include "Physics/Collision.h"
#include <queue>
#include <filesystem>
#include <unordered_set>
#include "NlohmannJson/json.hpp"
#include "Network/Connection.h"

using Json = nlohmann::json;

namespace TealEngine {
	class FrameBuffer;
	class Component;
	class GameNode
	{
	private:
		static std::unordered_set<GameNode*> allNodes;
		static std::map<unsigned int, GameNode*> idMap;
		static unsigned int lastId;
		static std::map<std::string, std::set<GameNode*>> tagged;
		static std::queue<GameNode*> destroyQueue;
		static GameNode* selectedNode;

		GameNode* sceneNode = nullptr;
		
		std::set<std::string> tags = std::set<std::string>();
		unsigned short activeMButtonsMask = 0;
		unsigned short hierarchyDepth = 0;
	protected:
		bool willBeDestroyed = false;
		std::string name = "";
		std::vector<GameNode*> childNodes = {};
		std::vector<Component*> components = {};
		GameNode* parent = nullptr;
		unsigned int id = 0;
		int hierarchyIndex = 0;
		bool active = true;
		bool onDestroyHasBeenCalled = false;
		bool showProperties = false;

		virtual void setParent(GameNode* parent);
		void destroyMark();
	public:
		inline unsigned char getMouseButtonPressMask() 
		{
			return activeMButtonsMask;
		};
		const std::string& getName();
		void rename(const std::string& name);
		inline const std::vector<Component*>& getComponents() { return components; };
		//checks if node has given tag added
		bool hasTag(const std::string& tag);
		//adds tag to a node
		void addTag(const std::string& tag);
		//removes tag from node
		void removeTag(const std::string& tag);
		//returns set of ALL nodes with given tag added
		static std::set<GameNode*> findNodesByTag(const std::string& tag);

		//component system

		//detaches component if it's attached to anything and attaches it to this node
		void attachComponent(Component* component);
		//detaches components if they attached to anything and attaches them to this node
		void attachComponents(const std::initializer_list<Component*>& component);
		//detaches component if component is attachet to this node, else throws an error
		void dettachComponent(Component* component);
		template<class T>
		//returns all components of given type
		std::vector<T*> findComponentsByType(bool recursive = false)
		{
			std::vector<T*> comps;
			for (Component* component : components)
				if (dynamic_cast<T*>(component))
					comps.push_back((T*)component);
			if(recursive) 
			{
				for(GameNode* child : childNodes) 
				{
					std::vector<T*> subComponents = child->findComponentsByType<T>(true);
					comps.insert(comps.end(), subComponents.begin(), subComponents.end());
				}
			}
			return comps;
		}

		template<class T>
		std::vector<const T*> findComponentsByTypeConst(bool recursive = false) const
		{
			std::vector<const T*> comps;
			for (const Component* component : components)
				if (dynamic_cast<const T*>(component))
					comps.push_back((const T*)component);
			if(recursive) 
			{
				for(const GameNode* child : childNodes) 
				{
					std::vector<const T*> subComponents = child->findComponentsByTypeConst<T>(true);
					comps.insert(comps.end(), subComponents.begin(), subComponents.end());
				}
			}
			return comps;
		}

		//returns length of path from root node to this node
		unsigned short getHierarchyDepth();
		//updates hierarcy depth of all child nodes
		void updateChildrenHierarcyDepths();

		//returns parent node, returns nullptr if node is an orphan
		GameNode* getParent();

		//adds child node
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
					std::vector<T*> childsOfChild = node->findChildsByType<T>(true);
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
			return castedParent;
		}
		//returns all child nodes
		std::vector<GameNode*> getChilds();
		//returns childs, childs of childs an so on in a single vector
		std::vector<GameNode*> getAllChilds();
		//returns childs with matching name
		std::vector<GameNode*> findChildsByName(std::string name);
		//node is active only if active property is true for it and all of its parents
		//use .getActiveThis to get this exact node activity property
		bool getActive() const;
		//returns value of node activity property
		//node can be innactive while having activity property set to true but any of it's parent nodes is innactive
		//use .getActive() to check if node is actually active
		inline bool getActiveThis() const { return this->active; };
		//events:

		//enables/disables node, calls onSleep or onAwake for components
		virtual void setActive(bool active);
		//called when parent changes
		virtual void onParentChange();
		//usualy called every frame
		virtual void update();
		//should be called after some mouse button was pressed and released while mouse cursor is hovered over the node
		virtual void onClick(unsigned short button);
		//called every time a mouse button is pressed while mouse cursor is hovered over the node
		virtual void onMousePress(unsigned short button);
		//called every time a mouse button is released
		virtual void onMouseRelease(unsigned short button);
		//called every time a mouse button is pressed
		virtual void onWindowMousePress(unsigned short button);
		//called every time a mouse button is released
		virtual void onWindowMouseRelease(unsigned short button);
		//
		virtual void onChildAdd(GameNode* child, GameNode* parent);
		//
		virtual void onChildRemove(GameNode* child, GameNode* parent);
		//
		virtual void onComponentAdd(Component* component, GameNode* node);
		//
		virtual void onComponentRemove(Component* component, GameNode* node);
		//
		void refreshGUI();
		virtual void setSceneNode(GameNode* sceneNode);
		inline GameNode* getSceneNode() { return sceneNode; };

		//recursively updates current and all lower nodes
		void updateAll();
		virtual void editorUpdate();
		
		void refreshProps();

		//Server side method
		//Should return true if you want to node to be 'visible' for given peer
		//Server will send node itself and messages to node to given peer if node is 'visible' for them
		//Should return false if server should not send any information about this node to given peer
		//Returned value for certain peer might change during lifetime of the node
		virtual bool isVisibleForPeer();

		//
		virtual void onCollision(const Collision& collision, bool eventDown = true, bool eventUp = true);
		//
		void GUIRender();
		//
		void imGuiRender(const std::string& windowName);

		//
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);

		virtual void displayNodeTree();

		virtual void displayProps();

		//Marks node for deletion
		//The node will be removed from the tree after all updates in current frame are finished
		//If called more than once all calls after the first will be ignored
		void destroy();
		//Deletes nodes marked for deletion
		static void cleanupDestroyed();

		GameNode();
		virtual ~GameNode();
		
		//Returns true if pointer is valid node and has not been deleted
		//Also returns true if node is marked for deletion by a 'destroy()' method call but has not been deleted yet
		static bool isValidNode(GameNode* node);

		virtual Json toJson();
		//returns node global id
		int getId();
		//set node id
		void setId(int id);
		//returns node by its id
		//returns nullptr if no node with such id was found
		static GameNode* getNodeById(int id);
		//returns currently selected node in editor
		static GameNode* getSelectedNode();
		//set current selected node in editor
		static void setSelectedNode(GameNode* node);
		//returns filepath from which this node was loaded from
		std::filesystem::path getNodeFilePath();
		//saves node to filepath it was loaded from
		void save();
		GameNode* getChildByIndex(int index);
		Component* getComponentByIndex(int index);
		//saves node to file
		void saveAs(const std::filesystem::path& path);
		static GameNode* loadNodeFromJsonFile(const std::filesystem::path& path);
		static GameNode* nodeFromJson(const Json& json);
		int getHierarchyIndex();
		std::vector<int> getRelativeIndexPathTo(GameNode* node);
	};
}