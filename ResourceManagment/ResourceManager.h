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

		inline bool exists(std::filesystem::path resourcePath) 
		{
			return loadedResources.find(resourcePath.string()) != loadedResources.cend();
		}
		
		T& get(std::filesystem::path resourcePath) 
		{
			auto it = loadedResources.find(resourcePath.string());
			if(it == loadedResources.cend()) 
			{
				load(resourcePath);
				return loadedResources[resourcePath.string()];
			}
			return it->second;
		};
		void load(std::filesystem::path resourcePath) 
		{
			loadImpl(resourcePath);
		}
		bool loadRecursive(std::filesystem::path folderPath) 
		{
			std::error_code ec;
			if(!std::filesystem::is_directory(folderPath, ec) && !std::filesystem::is_symlink(folderPath, ec))
				return false;
			for(const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) 
			{
				if(this->extensions.contains(entry.path().extension().string())) 
				{
					load(entry.path());
				}
			}

			return true;
		};

		bool unloadRecursive(std::filesystem::path folderPath) 
		{
			std::error_code ec;
			if(!std::filesystem::is_directory(folderPath, ec) && !std::filesystem::is_symlink(folderPath, ec))
				return false;
			for(const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) 
			{
				if(this->extensions.contains(entry.path().extension().string())) 
				{
					this->loadedResources.erase(entry.path().filename().string());
				}
			}

			return true;
		}
		virtual void clearAll() 
		{
			this->loadedResources.clear();
		}
	};
} // namespace TealEngine
