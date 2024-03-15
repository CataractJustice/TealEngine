#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "Core.h"
#include "Editor/Actions/PropSetAction.h"
#include "System/VTable.h"
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
			registerCallbackLists();
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
	void Component::onAwake() {};
	void Component::onDestroy() {};
	void Component::onParentChange() {};
	void Component::onSleep() {};
	void Component::onAttach() {};
	void Component::onMessageReceive() {};
	void Component::onCollision(const Collision& collision) {};
	void Component::GUIRender() {};
	void Component::imGuiRender(const std::string& windowName) {};
	void Component::render(ShaderProgram* shader, unsigned int stages) {};
	void Component::renderId() {};
	void Component::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer) {};
	void Component::onClick(unsigned short button) {};
	void Component::onWindowMousePress(unsigned short button) {};
	void Component::onWindowMouseRelease(unsigned short button) {};
	void Component::onMousePress(unsigned short button) {};
	void Component::onMouseRelease(unsigned short button) {};
	GameNode* Component::getParent() { return parent; }

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
		renderableComponents.erase(this);
		if (parent)
			parent->dettachComponent(this);

		for(auto& keyval : props) 
		{
			delete keyval.second;
		}
	}

	void Component::displayProps(bool& componentDeleted) 
	{
		if(this->name.length() == 0) return;
		std::vector<std::string> setProps;
		if(ImGui::CollapsingHeader((this->name + "###" + std::to_string((int)(long)this)).c_str()))
		{	
			if(ImGui::BeginDragDropSource()) 
			{
				ImGui::Text("%s", this->name.c_str());
				Component* lvalueThis = this;
				ImGui::SetDragDropPayload("ComponentInstance", &lvalueThis, sizeof(Component*));
				ImGui::EndDragDropSource();
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

				if(prop.second->display((prop.first + "##" + std::to_string((int)(long)this)).c_str())) 
				{
					setProps.push_back(prop.first);
					Json newValue;
					newValue["name"] = prop.first;
					newValue["value"] = prop.second->get();
					Core::actionList.pushAction(new PropSetAction(this, oldValue, newValue));
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
	}

	void Component::removeProp(const std::string& name) 
	{
		delete this->props[name];
		this->props.erase(name);
	}

	void Component::addProp(IProp* prop, const std::string& name) 
	{
		auto iterator = this->props.find(name);

		if(iterator != this->props.cend()) 
		{
			delete iterator->second;
			iterator->second = prop;
			return;
		}
		this->props[name] = prop;
		prop->setparentComponent(this);
	}

	Json Component::toJson() 
	{
		Json json;
		json["class"] = name;
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
		if(!component) return nullptr;
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

	void Component::renderAllComponents(ShaderProgram* shader, unsigned int stages) 
	{
		for(Component* comp : renderableComponents) 
		{
			if(comp->isActive())
				comp->render(shader, stages);
		}
	}

	void Component::registerCallbackLists() 
	{
		if(isMethodOverriden(Component::base, this, &Component::render)) renderableComponents.emplace(this);
	}

	std::map<int, Component*> Component::idMap;
	std::set<Component*> Component::allComponents;
	std::set<Component*> Component::renderableComponents;
	int Component::lastId = 1;
	Component Component::base;
}