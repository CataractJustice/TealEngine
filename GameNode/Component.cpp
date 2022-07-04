#include "Component.h"
namespace TealEngine 
{

	void Component::setFactoryName(std::string name) 
	{
		factoryName = name;
	}

	std::string Component::getFactoryName() 
	{
		return factoryName;
	}

	void Component::attachTo(GameNode* node) 
	{
		if (parrent != node) 
		{
			if (parrent)
				this->parrent->dettachComponent(this);
			this->parrent = node;
			onAttach();
		}
	}

	void Component::update() {};
	void Component::onAwake() {};
	void Component::onDestroy() {};
	void Component::onParrentChange() {};
	void Component::onSleep() {};
	void Component::onAttach() {};
	void Component::onMessageReceive() {};

	GameNode* Component::getParrent() { return parrent; }

	Component::~Component() 
	{
		if (parrent)
			parrent->dettachComponent(this);
	}
}