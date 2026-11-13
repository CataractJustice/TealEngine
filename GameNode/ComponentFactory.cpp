#include "ComponentFactory.h"
#include "FileSystem/getDirectoryList.h"
#include "System/Logger.h"
#include <format>
namespace TealEngine 
{
	Component* ComponentFactory::instanceComponent(const std::string& className) 
	{
		auto mangledNameIt = getMangledNames().find(className);
		if(mangledNameIt == getMangledNames().cend()) 
		{
			Logger::logInfo(std::format("No component named {}.", className));
			return nullptr;
		}

		Component* comp = getInstancers()[mangledNameIt->second]->newInstance();
		comp->rename(className);
		return comp;
	}

	const char** ComponentFactory::cStrComponentClassesArray() 
	{
		static const char* cStrArray[4096];
		int i = 0;
		for(auto& demangledMangledPair : getMangledNames())
		{
			cStrArray[i] = demangledMangledPair.first.c_str();
			i++;
		}
		return cStrArray;
	}

	int ComponentFactory::componentClassesCount() 
	{
		return ComponentFactory::getInstancers().size();
	}

	std::map<std::string, ComponentFactory::IComponentInstancer*>& ComponentFactory::getInstancers() 
	{
		static std::map<std::string, ComponentFactory::IComponentInstancer*> instancers = std::map<std::string, ComponentFactory::IComponentInstancer*>();
		return instancers;
	}
	std::map<std::string, std::string>& ComponentFactory::getMangledNames() 
	{
		static std::map<std::string, std::string> mangledNames = std::map<std::string, std::string>();
		return mangledNames;
	}
	
}