#ifndef COMPONENT_PROP_H
#define COMPONENT_PROP_H
#include <type_traits>
#include <string>
#include "libs/glm/glm.hpp"
#include "NlohmannJson/json.hpp"
#include "libs/imgui/imgui.h"
#include "libs/imgui/misc/cpp/imgui_stdlib.h"
#include <vector>
#include <functional>
using Json = nlohmann::json;

namespace TealEngine 
{
	class Component;
	/*
		A property interface
		Has methods for serialization and deserialization of object properties
	*/
	class IProp
	{
		private:
		protected:
		Component* ownerComp;
		Json storedJson;
		public:
		inline void setparentComponent(Component* comp) { ownerComp = comp; }
		void* propptr;
		//pointer to the property value
		//deserialize property
		virtual void set(const Json& serialized) = 0;
		//serialize property
		virtual Json get() = 0;
		//display property using ImGUI
		virtual bool display(const char* label) = 0;
		void storeJson(const Json& json);
		void refresh();

		void display(const char* label, Component* component);
		template<class T>
		inline T& valueref() { return *((T*)(propptr)); };
		virtual ~IProp() = default;
	};

	template<class T>
	class TProp : public IProp
	{
		public:
		void set(const Json& serialized) override
		{
			this->valueref<T>() = serialized.get<T>();
		}
		Json get() 
		{ 
			return Json(this->valueref<T>()); 
		}

		TProp(T* propptr) 
		{
			this->propptr = (void*)propptr;
		}
	};

	#define MacrosOverrideIProp \
	void set(const Json& serialized) override; \
	Json get() override; \
	bool display(const char* label) override;

	#define MacrosOverrideTProp \
	bool display(const char* label) override;\
	using TProp::TProp;
	
	
	class IntProp : public TProp<int> {public: MacrosOverrideTProp};
	class UIntProp : public TProp<unsigned int> {public: MacrosOverrideTProp};
	class ShortProp : public TProp<short> {public: MacrosOverrideTProp};
	class UShortProp : public TProp<unsigned short> {public: MacrosOverrideTProp};
	class CharProp : public TProp<char> {public: MacrosOverrideTProp};
	class LongProp : public TProp<long long> {public: MacrosOverrideTProp};
	class ULongProp : public TProp<unsigned long long> {public: MacrosOverrideTProp};
	class FloatProp : public TProp<float> {public: MacrosOverrideTProp};
	class StringProp : public  TProp<std::string> {public: MacrosOverrideTProp};
	class BoolProp : public  TProp<bool> {public: MacrosOverrideTProp};
	class FloatVecProp : public  IProp 
	{
		public:
		MacrosOverrideIProp
		int length;
		FloatVecProp(float* vecptr, int veclength);
	};
	class ColorProp : public FloatVecProp 
	{
		public: 
		bool display(const char* label) override;
		ColorProp(float* vecptr, int veclength = 4);
	};
	class VoidFunctionProp : public  IProp {public: MacrosOverrideIProp; inline VoidFunctionProp(void* propptr) {this->propptr = propptr;}};
	class EnumProp : public TProp<int> 
	{
		public:
		MacrosOverrideTProp;
		EnumProp(int* enumptr, const char** enumNames, int enumCount);
		private:
		const char** enumNames;
		int enumCount;
	};
	template <class MapValue>
	class StringKeyMapValuePointerProp : public IProp 
	{
		private:
		const std::map<std::string, MapValue>* mapptr;
		public:
		StringKeyMapValuePointerProp(const MapValue** propptr, const std::map<std::string, MapValue>* mapptr) : mapptr(mapptr)
		{
			this->propptr = (void*)propptr;
		}

		void set(const Json& json) override 
		{
			std::string key = json.get<std::string>();
			if((*mapptr).find(key) != (*mapptr).cend())
				this->valueref<const MapValue*>() = &(*((*mapptr).find(key))).second;
		}
		
		Json get() 
		{
			for(auto& keyval : *(this->mapptr)) 
			{
				if(this->valueref<MapValue*>() == &(keyval.second)) 
				{
					return keyval.first;
				}
			}
			return "";
		}

		bool display(const char* label) override 
		{
			std::string key = "";
			int mapsize = (*(this->mapptr)).size();
			const int staticKeysBufferSize = 1024;
			static char* keysStaticBuffer[staticKeysBufferSize]; 
			char** keys;
			if(mapsize > staticKeysBufferSize) 
			{
				keys = new char*[mapsize];
			} 
			else 
			{
				keys = keysStaticBuffer;
			}
			int index = -1;
			bool found = false;
			for(auto& keyval : *(this->mapptr)) 
			{
				index++;
				if(this->valueref<const MapValue*>() == &(keyval.second)) 
				{
					found = true;
					break;
				}
			}
			if(!found) index = -1;
			int i = 0;
			for(auto& keyval : *(this->mapptr)) 
			{
				keys[i++] = (char*)keyval.first.c_str();
			}

			bool edited = ImGui::Combo(label, &index, keys, mapsize);
			if(edited) 
			{
				this->valueref<const MapValue*>() = &(*this->mapptr->find(keys[index])).second;
			}
			if(mapsize > staticKeysBufferSize) delete[] keys;
			return edited;
		}
	};
}
#endif