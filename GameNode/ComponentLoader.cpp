#include "ComponentLoader.h"
#include <dlfcn.h>
#include "FileSystem/getDirectoryList.h"
namespace TealEngine 
{
	void ComponentLoader::loadDir(const std::string& path) 
	{
		std::vector<std::string> dlList = getDirectoryList(path);
		for(std::string& name : dlList) 
		{
			if(name.length() <= 3) 
			{
				TE_DEBUG_WARNING("Invalid file name (component skipped): " + std::string(name));
			}
			std::string componentName = name.substr(0, name.length() - 3);
			void* componentHandle = dlopen((path + "/" + name).c_str(), RTLD_LAZY);
			if(!componentHandle) 
			{
				std::cout << dlerror();
				continue;
			}
			componentHandles.push_back(componentHandle);
			void* createFn = dlsym(componentHandle, "create");

			char* symError = dlerror();

			if(symError) 
			{
				TE_DEBUG_WARNING("Error while loading 'create' method from " + name + " (component skipped): " + symError);
			}

			createFns[componentName] = (Component*(*)())createFn;
		}
	}
	Component* ComponentLoader::createComponent(std::string& name) 
	{
		return this->createFns[name]();
	}
}