#include "ComponentFactoryRegistrationList.h"
#include "ComponentFactory.h"

#include "NetworkComponents/DataComponent.h"
#include "NetworkComponents/NetworkTestComponent.h"

void TealEngine::componentFactoryRegisterComponents() 
{
	ComponentFactory::registerComponent<NetworkTestComponent>("NetworkTest");
}