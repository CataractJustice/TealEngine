#include "PropSetAction.h"

namespace TealEngine
{
	void PropSetAction::redo() 
	{
		Component::getComponentById(this->componentId)->setProp(newValue);
	}

	void PropSetAction::undo() 
	{
		Component::getComponentById(this->componentId)->setProp(oldValue);
	}

	PropSetAction::PropSetAction(Component* component, const Json& oldValue, const Json& newValue) 
	{
		this->oldValue = oldValue;
		this->newValue = newValue;
		this->componentId = component->getId();
	}
} // namespace TealEngine
