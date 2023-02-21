#include "ComponentFactory.h"
#include <dlfcn.h>
#include "FileSystem/getDirectoryList.h"
namespace TealEngine 
{
	Component* ComponentFactory::instanceComponent(const std::string& className) 
	{
		auto mangledNameIt = mangledNames.find(className);
		if(mangledNameIt == mangledNames.cend()) 
		{
			TE_DEBUG_ERROR("No component with class name \"" + className + "\" is registered, returning nullptr instead");
			return nullptr;
		}

		Component* comp = instancers[mangledNameIt->second]->newInstance();
		comp->rename(className);
		return comp;
	}

	const char** ComponentFactory::cStrComponentClassesArray() 
	{
		static const char* cStrArray[4096];
		int i = 0;
		for(auto& demangledMangledPair : mangledNames)
		{
			cStrArray[i] = demangledMangledPair.first.c_str();
			i++;
		}
		return cStrArray;
	}

	int ComponentFactory::componentClassesCount() 
	{
		return ComponentFactory::instancers.size();
	}

	std::map<std::string, ComponentFactory::IComponentInstancer*> ComponentFactory::instancers;
	std::map<std::string, std::string> ComponentFactory::mangledNames;
}