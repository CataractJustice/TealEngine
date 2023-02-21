#include "ComponentProp.h"
#include "Component.h"

namespace TealEngine 
{
	void IProp::display(const char* label, Component* component) 
	{
		if(this->display(label)) 
		{
			component->onPropSet(label);
		}
	}
}