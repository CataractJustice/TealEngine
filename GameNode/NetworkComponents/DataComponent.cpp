#include "DataComponent.h"
#include "System/Debug.h"
namespace TealEngine
{
	unsigned int DataComponent::idCounter = 0;
	unsigned int DataComponent::getID() { return id; }
	DataComponent::DataComponent(unsigned int id) 
	{
		if (id == 0)
		{
			idCounter++;
			this->id = idCounter;
		}
		else
			this->id = id;
	}
}