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
		inline void setParrentComponent(Component* comp) { ownerComp = comp; }
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
	class LongProp : public TProp<long> {public: MacrosOverrideTProp};
	class ULongProp : public TProp<unsigned long> {public: MacrosOverrideTProp};
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
		std::map<std::string, MapValue>* mapptr;
		public:
		StringKeyMapValuePointerProp(MapValue** propptr, std::map<std::string, MapValue>* mapptr) : mapptr(mapptr)
		{
			this->propptr = propptr;
		}

		void set(const Json& json) override 
		{
			std::string key = json.get<std::string>();
			if((*mapptr).find(key) != (*mapptr).cend())
				this->valueref<MapValue*>() = &((*mapptr)[key]);
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
			char** keys = mapsize > staticKeysBufferSize ? new char*[mapsize] : keysStaticBuffer;
			int index = -1;
			bool found = false;
			for(auto& keyval : *(this->mapptr)) 
			{
				index++;
				if(this->valueref<MapValue*>() == &(keyval.second)) 
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
			if(edited) this->valueref<MapValue*>() = &(this->mapptr->operator[](keys[index]));
			if(mapsize > staticKeysBufferSize) delete[] keys;
			return edited;
		}
	};
}
#endif