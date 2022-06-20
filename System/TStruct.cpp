#pragma once
#include <vector>
#include <string>
#include <map>
#include "Debug.h"
#include "TStruct.h"
#define HUFFMAN_IMPLEMENTATION
#include "libs/compression/huffman.hpp"
namespace TealEngine
{
	TStruct::TStructField& TStruct::TStructField::operator=(const TStruct::TStructField& val)
	{
		if (!isref && data)
			delete[] data;

		if (!val.isref)
		{
			data = new char[val.bytesize];
			memcpy(data, val.data, val.bytesize);
		}
		else
			data = val.data;

		isref = val.isref;
		bytesize = val.bytesize;
		return *this;
	};

	TStruct::TStructField::TStructField(const TStruct::TStructField& val)
	{	
		if (!val.isref) 
		{
			data = new char[val.bytesize];
			memcpy(data, val.data, val.bytesize);
		}
		else
			data = val.data;

		isref = val.isref;
		bytesize = val.bytesize;
	};

	TStruct::TStructField::TStructField() 
	{
		data = nullptr;
		bytesize = 0;
		isref = true;
	}

	TStruct::TStructField::TStructField(void* data, int bytesize, bool isref) : bytesize(bytesize), isref(isref)
	{
		this->bytesize - bytesize;
		if (isref)
			this->data = data;
		else
		{
			this->data = new char[bytesize];
			memcpy(this->data, data, bytesize);
		}
	}

	TStruct::TStructField::~TStructField() 
	{
		if (!isref && data && bytesize)
			delete[] data;
	}

	void* TStruct::TStructField::ptr() { return data; }
	unsigned int TStruct::TStructField::size() { return bytesize; }
	void TStruct::TStructField::set(void* val, unsigned int typebytesize, unsigned int index)
	{
		if (!data) 
		{
			bytesize = (typebytesize * index) + typebytesize;
			data = new char[bytesize];
		}
		memcpy((char*)data + (typebytesize * index), (char*)val, typebytesize);
	}

	void TStruct::TStructField::setref(void* val, unsigned int bytesize) 
	{
		this->data = val;
		this->bytesize = bytesize;
		this->isref = true;
	}

	void TStruct::TStructField::setcopy(void* val, unsigned int bytesize) 
	{
		if (!isref && data && bytesize)
			delete[] data;
		data = new char[bytesize];
		memcpy(data, val, bytesize);
	}

	TStruct::TStruct(const TStruct& other)
	{
		this->data = other.data;
	}

	TStruct& TStruct::operator=(const TStruct& other)
	{
		this->data = other.data;
		return *this;
	}

	TStruct::TStruct(char* data, unsigned int dataSize, bool decompress)
	{
		if (data != nullptr)
		{
			char* dec_data = data;
			unsigned int dec_size = dataSize;
			if (decompress)
			{
				dec_data = new char[*((unsigned int*)(data))];
				dec_size = *((int*)(data));
				int size_bits = (*((int*)(data) + 1));
				huffman::easyDecode((uint8_t*)data + sizeof(int) * 2, dataSize - sizeof(int) * 2, size_bits, (uint8_t*)dec_data, dec_size);

				//TE_DEBUG_LOG("compression ratio = " + to_string(double(dec_size) / double(dataSize)));
			}
			unsigned int cursor = 0;
			unsigned int typeSize = 0;
			while (cursor < dec_size)
			{
				memcpy(&typeSize, dec_data + cursor, sizeof(typeSize));
				cursor += sizeof(typeSize);
				char* val = new char[typeSize];
				memcpy(val, dec_data + cursor, typeSize);
				this->push(typeSize, val);
				cursor += typeSize;
			}
			if(decompress)
				delete[] dec_data;
		}
	}

	TStruct::~TStruct()
	{
		while (data.size())
		{
			this->pop();
		}
	}

	void TStruct::push(unsigned int size, void* data)
	{
		this->data.push_back(TStructField(data, size, false));
	}

	void TStruct::setFieldValue(unsigned int size, void* data, unsigned int index) 
	{
		this->data[index].set(data, size, 0);
	}

	void TStruct::pushReserve(int size) 
	{
		while (size) 
		{
			size--;
			data.push_back(TStructField());
		}
	}

	void TStruct::pop()
	{
		if (data.size() > 0)
			data.pop_back();
		else 
			TE_DEBUG_ERROR("Cannot pop empty TStruct");
	}

	void* TStruct::operator [](unsigned int index)
	{
		if (index < this->data.size())
			return data[index].ptr();
		else
		{
			TE_DEBUG_ERROR("Index out of bounds.");
			return nullptr;
		}
	}

	TStruct TStruct::operator +(TStruct second)
	{
		TStruct first = *this;
		for (unsigned int i = 0; i < second.data.size(); i++)
		{
			first.push(second.data[i]);
		}
		return first;
	}

	TStruct TStruct::operator+=(TStruct second)
	{
		*this = *this + second;
		return *this;
	}

	unsigned int TStruct::typeSize(unsigned int index)
	{
		return data[index].size();
	}

	std::string TStruct::getString(unsigned int index)
	{
		char* charStr = copyArray<char>(index);
		std::string str = std::string(charStr, typeSize(index) / sizeof(char));
		return str;
	}

	unsigned int TStruct::getFieldsCount()
	{
		return data.size();
	}

	std::string TStruct::getSchema() 
	{
		std::string schema = "[";
		for (unsigned int i = 0; i < data.size(); i++)
			schema += "{size: " + to_string(data[i].size()) + ", uint: " + to_string(getFieldValue<unsigned int>(i)) + ", int: " + to_string(getFieldValue<int>(i)) + ", string: \"" + getString(i).substr(0, 32) + "\"}" + ((i - data.size() + 1) ? "," : "");
		schema += "]";
		return schema;
	}

	uint8_t* TStruct::constructDataString(unsigned int& outDataSize) {
		uint8_t* data_string;
		int size = 0;
		for (TStructField& value : data)
		{
			size += value.size() + sizeof(value.size());
		}
		data_string = new uint8_t[size];
		unsigned int cursor = 0;
		for (TStructField value : data)
		{
			unsigned int bytesize = value.size();
			memcpy(data_string + cursor, &bytesize, sizeof(value.size()));
			cursor += sizeof(value.size());
			memcpy(data_string + cursor, value.ptr(), value.size());
			cursor += value.size();
		}

		outDataSize = size;
		return data_string;
	}


	void TStruct::clamp(int begin, int end) {
		std::vector<TStructField> temp;
		for (int i = begin; i < end; i++) {
			temp.push_back(data[i]);
		}
		data = temp;
	}
}