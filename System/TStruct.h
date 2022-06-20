#pragma once
#include <vector>
#include <string>
#include <stdexcept> 
#include "System/Debug.h"
#include "PreparedTStructSchema.h"
namespace TealEngine
{
	class TStruct
	{
	public:
		class TStructField 
		{
		private:
			void* data;
			unsigned int bytesize;
			bool isref;
		public:

			TStructField(const TStructField& val);
			TStruct::TStructField& operator=(const TStructField& val);

			template<class T>
			void operator=(T val)
			{
				if (!isref && data)
				{
					delete[] data;
					data = new char[bytesize];
				}

				if (bytesize == sizeof(T))
					memcpy(data, &val, bytesize);
				else
					TE_DEBUG_ERROR("Size of T doesn't match field size, use setcopy to set value of another type");
			}

			template<class T>
			void setcopy(T val)
			{
				if (!isref && data)
					delete[] data;
				bytesize = sizeof(val);
				data = new char[bytesize];
				memcpy(data, &val, bytesize);
			}
			
			void set(void* val, unsigned int typebytesize, unsigned int index = 0);
			void setcopy(void* val, unsigned int bytesize);
			void* ptr();
			unsigned int size();
			void setref(void* val, unsigned int bytesize);
			template<class T>
			void setref(T* val) 
			{
				if (!isref && data)
					delete[] data;
				data = val;
				isref = true;
			}
			template<class T>
			T* get(int index = 0) 
			{
					return (T*)(data + (index * sizeof(T)));
			}

			TStructField();
			TStructField(void* data, int bytesize, bool isref);
			~TStructField();
		};
	protected:
		std::vector<TStructField> data;
	public:
		TStruct(const TStruct& other);
		TStruct(char* data = nullptr, unsigned int dataSize = 0, bool decompress = false);
		TStruct& operator=(const TStruct& other);
		~TStruct();
		void push(unsigned int size, void* data);
		void setFieldValue(unsigned int size, void* data, unsigned int index);
		void pushReserve(int size);
		void pop();
		void* operator [](unsigned int index);
		TStruct operator +(TStruct second);
		TStruct operator+=(TStruct second);
		//creates new TStruct with elements of current TStruct from begin(including) to end(excluding)
		//TStruct& subStruct(int begin, int end);
		
		//deletes every field before begin and after end, end including
		void clamp(int begin, int end);
		unsigned int typeSize(unsigned int index);
		unsigned int getFieldsCount();
		std::string getString(unsigned int index);

		template<class T>
		void setRef(T* ref, unsigned int index) 
		{
			data[index].setref(ref);
		}

		template<class T>
		void push(T t) 
		{
			pushReserve(1);
			data.back().setcopy(t);
		}

		template<>
		void push<std::string>(std::string str)
		{
			this->data.push_back(TStructField((void*)str.c_str(), str.size(), false));
		}

		template<>
		void push<const char*>(const char* str)
		{
			this->data.push_back(TStructField((void*)str, std::strlen(str), false));
		}

		template <class T>
		T getFieldValue(unsigned int index) 
		{
			return *(T*)this->operator[](index);
		}

		template <class T>
		void setFieldValue(T t, unsigned int index)
		{
			data[index].setcopy(t);
		}

		template<>
		std::string getFieldValue(unsigned int index) 
		{
			return getString(index);
		}
		
		template<class T>
		T* copyArray(unsigned int index)
		{
			T* t = new T[typeSize(index) / sizeof(T)];
			memcpy(t, this->operator[](index), typeSize(index));
			return t;
		}

		uint8_t* constructDataString(unsigned int& outDataSize);

		std::string getSchema();
	};
}