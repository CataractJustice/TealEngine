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

using Json = nlohmann::json;

enum NODE_RELATION 
{
	THIS_NODE,
	NOT_RELATED_NODE,
	PARENT_NODE,
	CHILD_NODE
};

namespace TealEngine {
	class FrameBuffer;
	class ShaderProgram;
	class Component;
	class GameNode
	{
	private:
		static std::unordered_set<GameNode*> allNodes;
		static std::map<unsigned int, GameNode*> idMap;
		static unsigned int lastId;
		static map<string, set<GameNode*>> tagged;
		static std::queue<GameNode*> destroyQueue;
		static GameNode* selectedNode;
		
		set<string> tags;
		unsigned short activeMButtonsMask;
		unsigned short hierarchyDepth;
		bool willBeDestroyed;
	protected:
		string name;
		std::vector<GameNode*> childNodes;
		std::vector<Component*> components;
		GameNode* parent;
		unsigned int id;
		int hierarchyIndex;
		bool active;
		bool onDestroyHasBeenCalled;
		bool showProperties;

		virtual void setParent(GameNode* parent);
	public:
		inline unsigned char getMouseButtonPressMask() 
		{
			return activeMButtonsMask;
		};
		const string& getName();
		void rename(const string& name);

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

		//recursively updates current and all lower nodes
		void updateAll();
		void editorUpdate();
		
		void refreshProps();

		//Called whenever node recives a messgae, calls onMessageReceive(packet) for all components of this node
		virtual void onMessageReceive();
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
		virtual void render(ShaderProgram* shader = nullptr, unsigned int stages = 0);
		void renderId();

		//
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);

		virtual void displayNodeTree();

		virtual void displayProps();

		//Marks node for deletion
		//The node will be removed from the tree after all updates in current frame are finished
		//If called more than once all calls after the first will be ignored
		inline void destroy() { if (!willBeDestroyed) { destroyQueue.push(this); willBeDestroyed = true; } };
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