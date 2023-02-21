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
			TE_DEBUG_INFO("Loading shared library: \"" + path + "/" + name + "\"");
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