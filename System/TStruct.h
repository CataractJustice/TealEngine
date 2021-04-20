#pragma once
#include <vector>
#include <string>
namespace TealEngine
{
	class TStruct
	{
	private:
	protected:
		std::vector<std::pair<unsigned int, void*>> data;
	public:
		TStruct(const TStruct& other);
		TStruct(TStruct& other);
		TStruct(char* data = nullptr, unsigned int dataSize = 0, bool decompress = false);
		TStruct operator=(const TStruct& other);
		~TStruct();
		void push(unsigned int size, void* data);
		void push(std::string str);
		void pop();
		void* operator [](unsigned int index);
		TStruct operator +(TStruct second);
		TStruct operator+=(TStruct second);
		unsigned int typeSize(unsigned int index);
		unsigned int getFieldsCount();
		std::string getString(unsigned int index);

		template<class T>
		void push(T t) 
		{
			char* tCopy = new char[sizeof(T)];
			memcpy(tCopy, &t, sizeof(T));
			this->data.push_back(std::pair<unsigned int, void*>(sizeof(T), tCopy));
		}

		template <class T>
		T getFieldValue(unsigned int index) 
		{
			return *(T*)this->operator[](index);
		}

		
		template<class T>
		T* copyArray(unsigned int index)
		{
			T* t = new T[typeSize(index) / sizeof(T)];
			memcpy(t, this->operator[](index), typeSize(index));
			return t;
		}

		std::string getSchema();
	};
}