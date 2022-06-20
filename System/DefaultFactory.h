#pragma once
#include<map>
namespace TealEngine 
{
	template <class Key, class Base>
	class DefaultFactory 
	{
	private:
		template<class T>
		class Instantiator
		{
		public:
			T instantiate() { return new T(); }
		};

		std::map<Key, Base> registredTypes;
	public:
		template <class T>
		void registerType(Key key) { registredTypes.emplace(Instantiator<T>()); }
	};
}