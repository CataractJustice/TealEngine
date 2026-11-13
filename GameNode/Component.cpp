#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "Core.h"
#include "Editor/Actions/PropSetAction.h"
#include "System/VTable.h"
#include "GameNode/SceneNode.h"
namespace TealEngine 
{
	void Component::attachTo(GameNode* node) 
	{
		if (parent != node) 
		{
			if (parent)
				this->parent->dettachComponent(this);
			this->parent = node;
			if(Core::getEngineState() != Core::EngineState::GAME_STOPPED)
			this->onAttachCallbackHasBeenCalled = false;
		}
		if(!postConstructorDone) 
		{
			postConstructorDone = true;
		}
	}

	bool Component::isActive() 
	{
		return this->getParent()->getActive();
	}
	void Component::rename(const std::string& name) { this->name = name; };
	void Component::editorUpdate() {};
	void Component::update() {};
	void Component::onDestroy() {};
	void Component::onParentChange() {};
	void Component::onAwake() {};
	void Component::onSleep() {};
	void Component::onAttach() {};
	void Component::onCollision(const Collision& collision) {};
	void Component::GUIRender() {};
	void Component::imGuiRender(const std::string& windowName) {};
	void Component::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer) {};
	void Component::onClick(unsigned short button) {};
	void Component::onWindowMousePress(unsigned short button) {};
	void Component::onWindowMouseRelease(unsigned short button) {};
	void Component::onMousePress(unsigned short button) {};
	void Component::onMouseRelease(unsigned short button) {};
	void Component::onKeyPress(unsigned int key) {};
	void Component::onKeyRepeat(unsigned int key) {};
	void Component::onKeyRelease(unsigned int key) {};
	void Component::onChar(unsigned int unicode) {};
	void Component::onChildAdd(GameNode* node, GameNode* parent) {};
	void Component::onChildRemove(GameNode* node, GameNode* parent) {};
	void Component::onComponentAdd(Component* component, GameNode* parent) {};
	void Component::onComponentRemove(Component* component, GameNode* parent) {};
	void Component::onSceneChildAdd(GameNode* node, GameNode* parent) {};
	void Component::onSceneChildRemove(GameNode* node, GameNode* parent) {};
	void Component::onSceneComponentAdd(Component* component, GameNode* parent) {};
	void Component::onSceneComponentRemove(Component* component, GameNode* parent) {};
	void Component::onHostPeerConnect(PeerInfo& peerInfo) {};
	void Component::onHostPeerDisconnect(PeerInfo& peerInfo) {};
	void Component::onHostPeerPacket(PeerInfo& peerInfo, const void* data, int size) {};
	void Component::onClientConnect() {};
	void Component::onClientConnectFail() {};
	void Component::onClientDisconnect() {};
	void Component::onClientPacket(const void* data, int size) {};
	void Component::onPropRefresh() {};
	void Component::render(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) {};
	void Component::shadowRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) {};
	void Component::postRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) {};
	void Component::transparentRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) {};
	GameNode* Component::getParent() { return parent; }
	const GameNode* Component::getParentConst() const { return parent; }

	void Component::setNetworkId(NetworkId netId) 
	{
		if(this->getParent() && this->getParent()->getSceneNode()) 
		{
			((SceneNode*)this->getParent()->getSceneNode())->updateComponentNetworkId(this->netId, netId, this);
		}
		this->netId = netId;
	}

	Component::Component() : active(true), parent(nullptr) 
	{
		allComponents.emplace(this);
		this->id = lastId++;
		Component::idMap[id] = this;
		postConstructorDone = false;
	};

	bool Component::setProp(const Json& json) 
	{
		if(json.find("name") == json.cend())
			return false;
		if(json.find("value") == json.cend())
			return false;
		std::string name = json["name"];
		auto prop = this->props.find(name);
		if(prop == this->props.cend()) 
		{
			return false;
		}
		const Json& value = json["value"];
		prop->second->set(value);
		this->onPropSet(name);
		return true;
	}

	void Component::storeProp(const Json& json) 
	{
		if(json.find("name") == json.cend())
			return;
		if(json.find("value") == json.cend())
			return;
		std::string name = json["name"];
		auto prop = this->props.find(name);
		if(prop == this->props.cend()) 
		{
			return;
		}
		const Json& value = json["value"];
		prop->second->storeJson(value);
	}

	Component::~Component() 
	{
		allComponents.erase(this);
		if (parent)
			parent->dettachComponent(this);

		for(auto& keyval : props) 
		{
			delete keyval.second;
		}
		if(Input::Keyboard::getFocusedCharInput() == this) Input::Keyboard::focusCharInput(nullptr);
	}

	void Component::displayProps(bool& componentDeleted) 
	{
		std::string name = this->name;
		if(this->name.length() == 0) { name = "Unnamed"; }
		
		std::vector<std::string> setProps;
		if(ImGui::CollapsingHeader((name + "###" + std::to_string((int)(long long)this)).c_str()))
		{	
			if(ImGui::BeginDragDropSource()) 
			{
				ImGui::Text("%s", name.c_str());
				Component* lvalueThis = this;
				ImGui::SetDragDropPayload("ComponentInstance", &lvalueThis, sizeof(Component*));
				ImGui::EndDragDropSource();
			}
			if(
				isMethodOverriden(Component::base, this, &Component::onClientPacket) ||
				isMethodOverriden(Component::base, this, &Component::onHostPeerPacket)
			)
			{
				std::string comboLabel = "Network id label##" + std::to_string(this->getId());
				const char generatedIdLabel[] = "[Code Generated]";
				const std::vector<std::string>& labels = Core::networkDictionary.getLabels();
				const bool idIsGenerated = (this->netId[0] + 1 > labels.size()) || this->netId[1];
				const int intNetId = (int)this->netId[0];
				if(ImGui::BeginCombo(comboLabel.c_str(), idIsGenerated ? generatedIdLabel : labels[intNetId].c_str())) 
				{
					for(int i = 0; i < labels.size(); i++) 
					{
						if(ImGui::Selectable(labels[i].c_str(), intNetId == i)) 
						{
							setNetworkId({(long long)i, 0});
						}
					}
					ImGui::EndCombo();
				}
			}
			if(ImGui::BeginPopupContextItem()) 
			{
				if(ImGui::MenuItem("Delete component")) 
				{
					componentDeleted = true;
				}
				ImGui::EndPopup();
			}
			for(auto& prop : props) 
			{
				Json oldValue;
				oldValue["name"] = prop.first;
				oldValue["value"] = prop.second->get();

				if(prop.second->display((prop.first + "##" + std::to_string((int)(long long)this)).c_str())) 
				{
					setProps.push_back(prop.first);
					Json newValue;
					newValue["name"] = prop.first;
					newValue["value"] = prop.second->get();
					//As of right now it is a leak I think?
					//Core::actionList.pushAction(new PropSetAction(this, oldValue, newValue));
				}
			}
			for(const std::string& propName : setProps) 
			{
				this->onPropSet(propName);
			}
		}
	}

	void Component::onPropSet(const std::string& propName) {}
	
	void Component::refreshProps() 
	{
		for(auto prop : props) 
		{
			prop.second->refresh();
		}
		onPropRefresh();
	}

	void Component::removeProp(const std::string& name) 
	{
		delete this->props[name];
		this->props.erase(name);
	}

	void Component::addProp(IProp* prop, const std::string& name) 
	{
		prop->setparentComponent(this);
		auto iterator = this->props.find(name);

		if(iterator != this->props.cend()) 
		{
			delete iterator->second;
			iterator->second = prop;
			return;
		}
		this->props[name] = prop;
	}

	Json Component::toJson() 
	{
		Json json;
		json["class"] = name;
		if(netId[0] + 1 < Core::networkDictionary.getLabels().size()) 
		{
			json["netid"] = (int)netId[0];
		}
		std::vector<Json> propsJson;
		for(auto& prop : props) 
		{
			Json propJson;
			propJson["name"] = prop.first;
			propJson["value"] = prop.second->get();
			propsJson.push_back(propJson);
		}
		json["props"] = propsJson;
		return json;
	}

	Component* Component::fromJson(const Json& json) 
	{
		std::string componentClass = json["class"];
		Component* component = ComponentFactory::instanceComponent(componentClass);
		
		auto netIdIt = json.find("netid");
		if(!component) return nullptr;
		if(netIdIt != json.cend()) 
		{
			component->setNetworkId({netIdIt->get<int>(), 0});
		}
		auto propsIt = json.find("props");
		if(propsIt != json.cend()) 
		{
			for(const Json& propJson : *propsIt) 
			{
				component->storeProp(propJson);
			}
		}
		return component;
	}

	int Component::getId() 
	{
		return this->id;
	}
	
	void Component::setId(int id) 
	{
		Component::idMap.erase(this->id);
		auto newIdIt = idMap.find(id);
		if(newIdIt != Component::idMap.cend()) 
		{
			newIdIt->second->setId(this->id);
		}
		this->id = id;
		Component::idMap[id] = this;
	}

	Component* Component::getComponentById(int id) 
	{
		return idMap[id];
	}

	bool Component::isOnAttachCallbackHasBeenCalled() 
	{
		return onAttachCallbackHasBeenCalled;
	}

	void Component::cancelOnAttachCallback() 
	{
		onAttachCallbackHasBeenCalled = true;
	}

	bool Component::isValidComponent(Component* component) 
	{
		return allComponents.find(component) != allComponents.cend();
	}

	void Component::onWindowResize(int width, int height) {};

	std::map<int, Component*> Component::idMap;
	std::set<Component*> Component::allComponents;
	int Component::lastId = 1;
	Component Component::base;
}
