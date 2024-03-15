#include "GameNodeGUI.h"
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
		//solving the case where we for example have the following node hierarchy:
		//A
		//|--B
		//|  |--C
		//|
		//|--X
		//and we try to move A into the C node (Moving node to one of the child or child of child nodes)
		//
		//There is two possible behaviors that could be expected by a user
		//outcome of the first one is when we move branch leading to C out of the A node and make C A's parent:
		//B
		//|
		//C--A
		//   |--X
		//the second behavior is when we simply make node C A's parent:
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
		component->index = this->components.size() - 1;
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
			int i = 0;
			for(auto c : components) 
			{
				if(c == component) break;
				i++;
			}
			components.erase(components.begin() + i);
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
		node->hierarchyIndex = this->childNodes.size();
		node->setParent(this);
		this->childNodes.push_back(node);
		node->refreshProps();
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
			int i = 0;
			for(auto child : childNodes) 
			{
				if(child == node) break;
				i++;
			}
			this->childNodes.erase(childNodes.begin() + i);
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
			std::vector<GameNode*> childChildNodes = node->getAllChilds();
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
				if(!comp->isOnAttachCallbackHasBeenCalled()) 
				{
					comp->onAttach();
					comp->cancelOnAttachCallback();
				}
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
		childNodes = vector<GameNode*>();
		components = vector<Component*>();
		parent = NULL;
		hierarchyDepth = 0;
		hierarchyIndex = -1;
		allNodes.insert(this);
		active = true;
		id = 0;
		showProperties = false;
		activeMButtonsMask = 0;
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
		vector<GameNode*> nodes = childNodes;
		if(childNodes.size())
			for (GameNode* node : nodes)
			{
				delete node;
			}
		vector<Component*> comps = components;

		if (components.size())
			for (Component* comp : comps)
				delete comp;
				
		allNodes.erase(this);
		idMap.erase(this->getId());
	}

	bool GameNode::isValidNode(GameNode* node)
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

	void GameNode::GUIRender()
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->GUIRender();
		}

		for (GameNode* node : childNodes)
		{
			node->GUIRender();
		}
	}

	void GameNode::onClick(unsigned short button)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->onClick(button);
		}
	}

	void GameNode::onMouseRelease(unsigned short button)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->onMouseRelease(button);
		}
		if(this->activeMButtonsMask & (1 << button)) 
		{
			onClick(button);
		}
	}

	void GameNode::onMousePress(unsigned short button)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->onMousePress(button);
		}
		this->activeMButtonsMask |= 1 << button;
	}

	void GameNode::onWindowMouseRelease(unsigned short button)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->onWindowMouseRelease(button);
		}

		for (GameNode* node : childNodes)
		{
			node->onWindowMouseRelease(button);
		}
		this->activeMButtonsMask &= ~(1 << button);
	}

	void GameNode::onWindowMousePress(unsigned short button)
	{
		if (!active) return;
		for (Component* comp : components)
		{
			if (comp->getActive())
				comp->onWindowMousePress(button);
		}

		for (GameNode* node : childNodes)
		{
			node->onWindowMousePress(button);
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
		Component::renderAllComponents(shader, stages);
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

	void GameNode::cleanupDestroyed() 
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

		if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(0)) 
		{
			GameNode::setSelectedNode(this);
		}
		
		if(ImGui::BeginPopupContextItem()) 
		{
			if(ImGui::MenuItem("Add empty 3D node")) 
			{
				GameNode* newNode = new GameNode3D();
				this->addChild(newNode);
				Core::actionList.pushAction(new NodeCreateAction(newNode));
			}
			if(ImGui::MenuItem("Add empty GUI node")) 
			{
				GameNode* newNode = new GameNodeGUI();
				this->addChild(newNode);
				Core::actionList.pushAction(new NodeCreateAction(newNode));
			}
			if(ImGui::MenuItem("Add empty node")) 
			{
				GameNode* newNode = new GameNode();
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
			if(ImGui::MenuItem("Show properties", nullptr, &showProperties)) 
			{
			}
			ImGui::EndPopup();
		}

		if(this->showProperties) 
		{
			ImGui::Begin((this->name + " properties").c_str(), &this->showProperties);
			this->displayProps();
			ImGui::End();
		}
		
		//if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hi");
		
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
		json["type"] = "base";
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

	void GameNode::saveAs(const std::filesystem::path& path) 
	{
		std::ofstream savefile(path);
		savefile << this->toJson();
		savefile.close();
	}

	GameNode* GameNode::nodeFromJson(const Json& json) 
	{
		auto typeIt = json.find("type");
		GameNode* node = nullptr;

		// Check if "type" is found in json, if not assume value of "3d"
		if (typeIt == json.cend() || typeIt->get<std::string>() == "3d") 
		{
			node = new GameNode3D();
		} else if (typeIt->get<std::string>() == "base") 
		{
			node = new GameNode();
		} else if (typeIt->get<std::string>() == "gui") 
		{
			node = new GameNodeGUI();
		} else 
		{
			//if unrecognized assume 3d
			node = new GameNode3D();
		}

		auto componentsIt = json.find("components");
		if (componentsIt != json.cend()) 
		{
			for (const Json& componentJson : *componentsIt) 
			{
				Component* comp = Component::fromJson(componentJson);
				if (comp) {
					node->attachComponent(comp);
				}
			}
		}

		if (json.find("nodes") != json.cend()) 
		{
			const Json& subNodes = json["nodes"];
			for (const Json& subNodeJson : subNodes) {
				GameNode* subNode = nodeFromJson(subNodeJson);
				node->addChild(subNode);
			}
		}

		if (json.find("name") != json.cend()) 
		{
			node->rename(json["name"]);
		}

		if (typeIt == json.cend() || typeIt->get<std::string>() == "3d") 
		{
			GameNode3D* node3D = (GameNode3D*)node;
			node3D->transformProp->set(json["transform"]);
		} 
		else if (typeIt->get<std::string>() == "base") 
		{
		} 
		else if (typeIt->get<std::string>() == "gui") 
		{
			GameNodeGUI* nodeGui = (GameNodeGUI*)node;
			nodeGui->transformProp->set(json["transform"]);
		}
		return node;
	}
	
	GameNode* GameNode::loadNodeFromJsonFile(const std::filesystem::path& path) 
	{
		std::ifstream file(path);
		GameNode* node = GameNode::nodeFromJson(Json::parse(file));
		return node;
	}

	GameNode* GameNode::getChildByIndex(int index) 
	{
		if(index >= this->childNodes.size()) return nullptr;
		return this->childNodes[index];
	}
	Component* GameNode::getComponentByIndex(int index) 
	{
		if(index >= this->components.size()) return nullptr;
		return this->components[index];
	}
	
	std::vector<int> GameNode::getRelativeIndexPathTo(GameNode* node) 
	{
		if(node == nullptr) return std::vector<int>();
		int depthA = this->getHierarchyDepth();
		int depthB = node->getHierarchyDepth();
		GameNode* itA = this;
		GameNode* itB = node;

		std::vector<int> pathA;
		std::vector<int> pathB;

		if(depthA < depthB) 
		{
			for(int i = depthB - depthA; i; i--) 
			{
				pathB.push_back(itB->getHierarchyIndex());
				itB = itB->getParent();
			}
		}
		else 
		{
			for(int i = depthA - depthB; i; i--) 
			{
				pathA.push_back(-1);
				itA = itA->getParent();
			}
		}

		while(itA && itB && itA != itB) 
		{
			pathA.push_back(-1);
			pathB.push_back(itB->getHierarchyIndex());
			itA = itA->getParent();
			itB = itB->getParent();
		}

		if(itA && itB && itA == itB) 
		{
			for(int i = pathB.size() - 1; i >= 0; i--) 
			{
				pathA.push_back(pathB[i]);
			}
			return pathA;
		}
		return std::vector<int>();
	}
	
	int GameNode::getHierarchyIndex() 
	{
		return this->hierarchyIndex;
	}

	std::map<unsigned int, GameNode*> GameNode::idMap;
	std::queue<GameNode*> GameNode::destroyQueue;
	unsigned int GameNode::lastId = 1;
	GameNode* GameNode::selectedNode = nullptr;
	GameNode* GameNode::getSelectedNode() { return GameNode::selectedNode;  }
	void GameNode::setSelectedNode(GameNode* node) { GameNode::selectedNode = node; }
}
