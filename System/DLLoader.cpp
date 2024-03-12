#include "DLLoader.h"
#include "Debug.h"
#include <dlfcn.h>
#include "FileSystem/getDirectoryList.h"
namespace TealEngine 
{
	void DLLoader::loadDir(const std::string& path) 
	{
		std::vector<std::string> dlList = getDirectoryList(path);
		for(std::string& name : dlList) 
		{
			if(name.length() <= 3) continue;
			if(name.substr(name.length() - 3, 3) != ".so") continue;
			void* dlhandle = dlopen((path + "/" + name).c_str(), RTLD_NOW);
			if(!dlhandle) 
			{
				char* loaderror = dlerror();
				TE_DEBUG_WARNING("Error while loading shared library: " + loaderror);
				continue;
			}

			handles[path + name] = dlhandle;
		}
	}

	void DLLoader::unloadAll() 
	{
		for(auto& handle : handles) 
		{
			dlclose(handle.second);
		}
		handles.clear();
	}

	DLLoader::~DLLoader() 
	{
		unloadAll();
	}
}