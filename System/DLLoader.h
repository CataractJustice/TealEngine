#pragma once
#include <string>
#include <map>
namespace TealEngine 
{
	class DLLoader 
	{
		private:
			std::map<std::string, void*> handles;
		public:
		void loadDir(const std::string& path);
		void unloadAll();

		~DLLoader();
	};
}