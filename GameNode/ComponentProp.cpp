#include "ComponentProp.h"
#include "Component.h"

namespace TealEngine 
{
	void IProp::display(const char* label, Component* component) 
	{
		if(this->display(label) && component) 
		{
			component->onPropSet(label);
		}
	}

	void IProp::refresh() 
	{
		if(this->storedJson.empty()) return;
		this->set(this->storedJson);
	}
	void IProp::storeJson(const Json& json) 
	{
		this->storedJson = json;
	}
}