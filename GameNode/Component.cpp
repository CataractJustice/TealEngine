#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "Core.h"
#include "Editor/Actions/PropSetAction.h"
namespace TealEngine 
{
	void Component::attachTo(GameNode* node) 
	{
		if (parent != node) 
		{
			if (parent)
				this->parent->dettachComponent(this);
			this->parent = node;
			onAttach();
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
	void Component::onCollision(const Physics::Collision& collision) {};
	void Component::GUIrender() {};
	void Component::imGuiRender(const std::string& windowName) {};
	void Component::render(ShaderProgram* shader, unsigned int stages) {};
	void Component::renderId() {};
	void Component::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light) {};

	GameNode* Component::getParent() { return parent; }

	Component::Component() : active(true), parent(nullptr) 
	{
		this->id = lastId++;
		Component::idMap[id] = this;
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

	Component::~Component() 
	{
		if (parent)
			parent->dettachComponent(this);

		for(auto& keyval : props) 
		{
			delete keyval.second;
		}
	}

	void Component::displayProps() 
	{
		if(this->name.length() == 0) return;
		std::vector<std::string> setProps;
		if(ImGui::CollapsingHeader((this->name + "###" + std::to_string((int)(long)this)).c_str()))
		{	
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
				component->setProp(propJson);
			}
			for(const Json& propJson : *propsIt) 
			{
				component->setProp(propJson);
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

	std::map<int, Component*> Component::idMap;
	int Component::lastId = 1;
}