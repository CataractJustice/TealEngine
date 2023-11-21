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

			static std::map<std::string, IComponentInstancer*> instancers;
			static std::map<std::string, std::string> mangledNames;
		public:
		template<class T>
		static void registerComponent(const std::string& demangledName) 
		{
			std::string mangledName = typeid(T).name();
			auto instancerIt = instancers.find(mangledName);
			mangledNames[demangledName] = mangledName;
			if(instancerIt == instancers.cend())
				instancers[mangledName] = new ComponentInstancer<T>();
		}

		template<class T>
		static void unregisterComponent() 
		{
			instancers.erase(typeid(T).name());
			for(auto it = mangledNames.begin(); it != mangledNames.end(); it++) 
			{
				if(it->second == typeid(T).name()) 
				{
					mangledNames.erase(it);
					break;
				}
			}
		}

		static Component* instanceComponent(const std::string& className);
		static const char** cStrComponentClassesArray();
		static int componentClassesCount();
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