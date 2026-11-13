#pragma once
#include "Component.h"
#include <map>
namespace TealEngine 
{
	class ComponentFactory 
	{
		private:
			class IComponentInstancer 
			{
				public:
				virtual Component* newInstance() = 0;
			};

			template<class T> 
			class ComponentInstancer : public IComponentInstancer
			{
				Component* newInstance()
				{
					return new T();
				}
			};

		static std::map<std::string, IComponentInstancer*>& getInstancers();
		static std::map<std::string, std::string>& getMangledNames();
		public:
		template<class T>
		static void registerComponent(const std::string& demangledName) 
		{
			std::string mangledName = typeid(T).name();
			auto instancerIt = getInstancers().find(mangledName);
			getMangledNames()[demangledName] = mangledName;
			if(instancerIt == getInstancers().cend())
				getInstancers()[mangledName] = new ComponentInstancer<T>();
		}

		template<class T>
		static void unregisterComponent() 
		{
			delete getInstancers()[typeid(T).name()];
			getInstancers().erase(typeid(T).name());
			for(auto it = getMangledNames().begin(); it != getMangledNames().end(); it++) 
			{
				if(it->second == typeid(T).name()) 
				{
					getMangledNames().erase(it);
					break;
				}
			}
		}

		static Component* instanceComponent(const std::string& className);
		static const char** cStrComponentClassesArray();
		static int componentClassesCount();
		~ComponentFactory() 
		{
			for(auto instancer : getInstancers()) 
			{
				delete instancer.second;
			}
		}
	};

	template<class T>
	class ComponentFactoryAdder 
	{
		public:
		ComponentFactoryAdder(const std::string& demangledName) 
		{
			ComponentFactory::registerComponent<T>(demangledName);
		}

		~ComponentFactoryAdder() 
		{
			ComponentFactory::unregisterComponent<T>();
		}
	};
}

#define EXPORT_COMPONENT(class) extern "C" { TealEngine::ComponentFactoryAdder<class> __componentFactoryAdder##class = TealEngine::ComponentFactoryAdder<class>(#class); }