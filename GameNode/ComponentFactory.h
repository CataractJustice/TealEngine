#pragma once
#include "Component.h"
#include <map>
namespace TealEngine 
{
	class ComponentFactory
	{
	private:

		class ComponentInstantiator
		{
		public:
			virtual Component* instantiate() = 0;
		};

		template<class T>
		class ComponentInstantiatorT : public ComponentInstantiator
		{
		public:
			Component* instantiate() 
			{
				return new T; 
			}
		};

		static std::map<std::string, ComponentInstantiator*> componentInstantiators;

	public:
		template<class T>
		static void registerComponent(std::string name)
		{
			componentInstantiators[name] = new ComponentInstantiatorT<T>();
		}

		static Component* instantiateComponent(std::string componentName) 
		{
			Component* c = componentInstantiators[componentName]->instantiate();
			c->setFactoryName(componentName);
			return c;
		}
	};
}