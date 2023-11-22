#include "GameNode3D.h"
#include <unordered_set>
#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "libs/tinyfiledialogs.h"
#include <fstream>
#include "Core.h"
#include "Editor/Actions/NodeCreateAction.h"
#include "Editor/Actions/NodeDeleteAction.h"
#include "Core.h"
#include "Graphics/Renderer/Shapes/Line3D.h"
using namespace std;
namespace TealEngine {

	unordered_set<GameNode*> GameNode::allNodes = unordered_set<GameNode*>(8192);
	map<string, set<GameNode*>> GameNode::tagged = map<string, set<GameNode*>>();

	//sets parent of a node, calls GameNode::onParentChange after setting new parent
	void GameNode::setParent(GameNode* parent)
	{
		//solving the case where we have the following node hierarchy:
		//A
		//|--B
		//|  |--C
		//|
		//|--X
		//and we try to move A into a C node (Moving node to one of the child nodes)
		//
		//There is two possible behaviours that could be expected by a user
		//outcome of the first one:
		//B
		//|
		//C--A
		//   |--X
		//the second:
		//C
		//|--A
		//   |--B
		//   |--X
		//
		//I will choose the first behaviour 
		GameNode* parentIter = parent;
		while(parentIter) 
		{
			if(parentIter->getParent() == this) 
			{
				this->removeChild(parentIter);
				if(this->getParent()) this->getParent()->addChild(parentIter);
				break;
			}
			parentIter = parentIter->getParent();
		}
		if (this->parent && parent)
			this->parent->removeChild(this);
		this->parent = parent;
		
		if(this->parent)
			this->hierarchyDepth = parent->getHierarchyDepth() + 1;
		else
			hierarchyDepth = 0;

		updateChildrenHierarcyDepths();

		onParentChange();
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
		if (component->getParent() == this) 
		{
			components.remove(component);
			component->parent = nullptr;
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

	void GameNode::updateChildrenHierarcyDepths() 
	{
		for(GameNode* child : this->childNodes) 
		{
			child->hierarchyDepth = this->hierarchyDepth + 1;
			child->updateChildrenHierarcyDepths();
		}
	}

	GameNode* GameNode::addChild(GameNode* node)
	{
		node->setParent(this);
		this->childNodes.push_back(node);
		return childNodes.back();
	}

	void GameNode::addChilds(const std::initializer_list<GameNode*>& nodes)
	{
		for (auto& node : nodes) this->addChild(node);
	}

	void GameNode::removeChild(GameNode* node)
	{
		if (node->getParent() == this) 
		{
			this->childNodes.remove(node);
			node->setParent(nullptr);
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

	GameNode* GameNode::getParent()
	{
		return parent;
	}
	bool GameNode::getActive() { return active && (getParent() ? getParent()->getActive() : true); }
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
	void GameNode::onParentChange() 
	{
		for (Component* comp : components)
			comp->onParentChange();
	};
	void GameNode::update()
	{
		if (!active) return;
		for (Component* comp : components)
			if(comp->getActive()) 
			{
				comp->update();
				comp->editorUpdate();
			}
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

	void GameNode::editorUpdate() 
	{
		if(!active) return;

		for(GameNode* node : childNodes) 
		{
			node->editorUpdate();
		}
		for(Component* component : components) 
		{
			component->editorUpdate();
		}
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
		parent = NULL;
		hierarchyDepth = 0;
		allNodes.insert(this);
		active = true;
		id = 0;
		groupId = 0;
		groupLocalId = 0;
		this->loadedFromFilePath = "";
	}

	GameNode::~GameNode()
	{
		if(GameNode::selectedNode == this) GameNode::selectedNode = nullptr;
		//removing node from parent child nodes
		if (parent) parent->removeChild(this);
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
		idMap.erase(this->getId());
		groupsIdMap[this->groupId].erase(this->groupLocalId);
		if(groupsIdMap[this->groupId].size() == 0) 
		{
			groupsIdMap.erase(this->groupId);
		}
	}

	bool GameNode::isNodeExist(GameNode* node)
	{
		return allNodes.find(node) != allNodes.end();
	}

	bool GameNode::isVisibleForPeer() 
	{
		return true;
	}

	void GameNode::onCollision(const Collision& collision, bool eventDown, bool eventUp) 
	{
		if(eventDown && eventUp) 
		{
			Core::shapesRenderer.pushShape(Line3D(collision.pos, collision.pos + collision.normal*0.1f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.01f));
			Core::shapesRenderer.pushShape(Line3D(collision.pos + collision.normal*0.1f, collision.pos + collision.normal, glm::vec4(0.0, 1.0f, 1.0f, 1.0f), 0.01f));
		}

		if (!active) return;
		for (Component* comp : components) 
		{
			if (comp->getActive())
				comp->onCollision(collision);
		}

		if(eventUp && this->parent)
			this->parent->onCollision(collision, false);

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

	void GameNode::imGuiRender(const std::string& windowName)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->imGuiRender(windowName);
		}

		for (GameNode* node : childNodes)
		{
			node->imGuiRender(windowName);
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

	void GameNode::renderId()
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->renderId();
		}

		for (GameNode* node : childNodes)
		{
			node->renderId();
		}
	}

	void GameNode::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (comp->getActive())
				comp->postProcess(unlitColor, litColor, position, normal, specular, light, frameBuffer);
		}

		for (GameNode* node : childNodes)
		{
			node->postProcess(unlitColor, litColor, position, normal, specular, light, frameBuffer);
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


	void GameNode::displayNodeTree() 
	{
		ImGuiTreeNodeFlags nodeFlags = 
		ImGuiTreeNodeFlags_AllowItemOverlap | 
		ImGuiTreeNodeFlags_OpenOnArrow | 
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;
		if(GameNode::getSelectedNode() == this) { nodeFlags |= ImGuiTreeNodeFlags_Selected; }

		std::string nodeLabel = (this->name.length() ? this->name : std::string("unnamed"));
		std::string nodeId = std::to_string((long)this);

		bool treeOpen = ImGui::TreeNodeEx((nodeLabel + "###" + nodeId).c_str(), nodeFlags);

		if(ImGui::BeginDragDropSource()) 
		{
			ImGui::Text("%s", this->name.c_str());
			GameNode* lvalueThis = this;
			ImGui::SetDragDropPayload("GameNode", &lvalueThis, sizeof(GameNode*));
			ImGui::EndDragDropSource();
		}

		if(ImGui::BeginDragDropTarget()) 
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameNode")) 
			{
				GameNode* childNode = *((GameNode**)payload->Data);
				if(childNode != this && childNode->parent != this) 
				{
					this->addChild(childNode);
				}
			}

			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NodeFilePath")) 
			{
				GameNode* loadedNode = GameNode3D::loadNodeFromJsonFile((char*)payload->Data);
				this->addChild(loadedNode);
			}
			ImGui::EndDragDropTarget();
		}

		if(ImGui::IsItemClicked(0)) 
		{
			GameNode::setSelectedNode(this);
		}
		
		if(ImGui::BeginPopupContextItem()) 
		{
			if(ImGui::MenuItem("Add empty child")) 
			{
				GameNode* newNode = new GameNode3D();
				this->addChild(newNode);
				Core::actionList.pushAction(new NodeCreateAction(newNode));
			}
			if(this->getParent()) 
			{
				if(ImGui::MenuItem("Delete this node and its childs")) 
				{
					Core::actionList.pushAction(new NodeDeleteAction(this)); 
					this->destroy();
				};
			}
			ImGui::EndPopup();
		}
		
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete node");
		
		if(treeOpen)
		{
			ImGui::InputText("name", &this->name);
			ImGui::Checkbox("Active", &active);
			auto childNodesTemp = this->childNodes;
			for(GameNode* node : childNodesTemp) 
			{
				if(node->getParent() == this)
					node->displayNodeTree();
			}
			ImGui::TreePop();
		}
	}

	Json GameNode::toJson() 
	{
		Json json;
		json["name"] = this->name;
		json["localId"] = this->groupLocalId;
		json["group"] = this->groupId;
		json["filepath"] = this->loadedFromFilePath;
		std::vector<Json> componentsJson;
		for(Component* comp : components) 
		{
			componentsJson.push_back(comp->toJson());
		}
		json["components"] = componentsJson;

		std::vector<Json> nodesJson;
		for(GameNode* node : childNodes) 
		{
			nodesJson.push_back(node->toJson());
		}
		json["nodes"] = nodesJson;
		return json;
	}

	int GameNode::getId() 
	{
		if(!id) 
		{
			while(idMap.find(++lastId) != idMap.cend());
			idMap[lastId] = this;
			id = lastId;
		}
		return this->id;
	}
	
	void GameNode::setId(int id) 
	{
		GameNode::idMap.erase(this->id);
		auto newIdIt = idMap.find(id);
		if(newIdIt != GameNode::idMap.cend()) 
		{
			return;
		}
		this->id = id;
		GameNode::idMap[id] = this;
	}

	void GameNode::displayProps() 
	{
		std::vector<Component*> deletedComponents;
		for(Component* comp : components) 
		{
			bool componentDeleted = false;
			comp->displayProps(componentDeleted);
			if(componentDeleted) 
			{
				deletedComponents.push_back(comp);
			}
		}
		for(Component* deletedComp : deletedComponents) 
		{
			this->dettachComponent(deletedComp);
			delete deletedComp;
		}
	}

	void GameNode::refreshProps() 
	{
		for (Component* comp : components)
		{
				comp->refreshProps();
		}

		for (GameNode* node : childNodes)
		{
			node->refreshProps();
		}
	}

	GameNode* GameNode::getNodeById(int id) 
	{
		auto it = GameNode::idMap.find(id);
		return ((it == GameNode::idMap.cend()) ? nullptr : it->second);
	}

	void GameNode::setGroupAndLocalId(int groupId, int groupLocalId) 
	{
		this->groupId = groupId;
		this->groupLocalId = groupLocalId;
		GameNode::groupsIdMap[groupId][groupLocalId] = this;
	}

	unsigned int GameNode::getGroupLocalId() 
	{
		return this->groupLocalId;
	}

	unsigned int GameNode::getGroupId() 
	{
		return this->groupId;
	}

	GameNode* GameNode::getNodeByGroupAndLocalId(int groupId, int groupLocalId) 
	{
		if(GameNode::groupsIdMap.find(groupId) != GameNode::groupsIdMap.cend()) 
		{
			if(groupsIdMap[groupId].find(groupLocalId) != GameNode::groupsIdMap[groupId].cend()) 
			{
				return GameNode::groupsIdMap[groupId][groupLocalId];
			}
		}
		return nullptr;
	}

	void GameNode::setNodeFilePath(std::filesystem::path path) 
	{
		this->loadedFromFilePath = path.string();
	}
	
	std::filesystem::path GameNode::getNodeFilePath() 
	{
		return this->loadedFromFilePath;
	}

	void GameNode::save() 
	{
		if(loadedFromFilePath.length() != 0)
		this->saveAs(this->loadedFromFilePath);
	}
	void GameNode::saveAs(const std::filesystem::path& path) 
	{
		std::ofstream savefile(path);
		savefile << this->toJson();
		savefile.close();
	}

	std::map<unsigned int, GameNode*> GameNode::idMap;
	std::map<unsigned int, std::map<unsigned int, GameNode*>> GameNode::groupsIdMap;
	std::queue<GameNode*> GameNode::destroyQueue;
	unsigned int GameNode::lastId = 1;
	GameNode* GameNode::selectedNode = nullptr;
	GameNode* GameNode::getSelectedNode() { return GameNode::selectedNode;  }
	void GameNode::setSelectedNode(GameNode* node) { GameNode::selectedNode = node; }
}