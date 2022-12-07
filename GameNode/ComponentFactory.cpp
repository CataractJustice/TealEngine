#include "ComponentFactory.h"
namespace TealEngine
{
	std::map<std::string, ComponentFactory::ComponentInstantiator*> ComponentFactory::componentInstantiators = std::map<std::string, ComponentFactory::ComponentInstantiator*>();
}