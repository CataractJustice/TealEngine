#include "ComponentAddAction.h"
#include "GameNode/Component.h"

namespace TealEngine 
{
	void ComponentAddAction::redo() 
	{
		this->component = Component::fromJson(this->componentJson);
		GameNode::getNodeById(this->nodeId)->attachComponent(component);
	}

	void ComponentAddAction::undo() 
	{
		this->componentJson = component->toJson();
		this->component->getParent()->dettachComponent(component);
		delete component;
	}

	ComponentAddAction::ComponentAddAction(Component* component) 
	{
		this->component = component;
		this->nodeId = component->getParent()->getId();
	}
}