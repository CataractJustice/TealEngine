#pragma once
#include <filesystem>
#include <set>
#include <map>
#include "System/Debug.h"

namespace TealEngine
{
	template <class T>
	class ResourceManager 
	{
		protected:
		std::set<std::string> extensions;
		std::map<std::string, T> loadedResources;

		virtual void loadImpl(std::filesystem::path resourcePath) = 0;

		public:

		std::map<std::string, T>& getLoadedMap() 
		{
			return loadedResources;
		}
		
		T& get(std::filesystem::path resourcePath) 
		{
			auto it = loadedResources.find(resourcePath);
			if(it == loadedResources.cend()) 
			{
				load(resourcePath);
				return loadedResources[resourcePath];
			}
			return it->second;
		};
		void load(std::filesystem::path resourcePath) 
		{
			if(this->extensions.contains(resourcePath.extension())) 
			{
				loadImpl(resourcePath);
			}
			else 
			{
				TE_DEBUG_ERROR("Unsupported file extension");
			}
		}
		virtual int loadRecursive(std::filesystem::path folderPath) 
		{
			if(!std::filesystem::is_directory(folderPath))
				return -1;
			for(const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) 
			{
				if(this->extensions.contains(entry.path().extension().string())) 
				{
					load(entry.path());
				}
			}

			return 0;
		};
		virtual void clearAll() 
		{
			this->loadedResources.clear();
		}
	};
} // namespace TealEngine
