#include "Component.h"
#include <map>
namespace TealEngine 
{
	class ComponentLoader 
	{
		private:
			std::map<std::string, Component*(*)()> createFns;
			std::vector<void*> componentHandles;
		public:
		void loadDir(const std::string& path);

		Component* createComponent(std::string& name);
	};
}