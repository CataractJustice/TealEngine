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

	TStruct::TStruct(const TStruct& other)
	{
		this->data = vector<std::pair<unsigned int, void*>>();
		for (unsigned int i = 0; i < other.data.size(); i++)
		{
			data.push_back(std::pair<unsigned int, void*>(other.data[i].first, new char[other.data[i].first]));
			memcpy(this->data[i].second, other.data[i].second, other.data[i].first);
		}
	}

	TStruct::TStruct(TStruct& other)
	{
		this->data = vector<std::pair<unsigned int, void*>>();
		for (unsigned int i = 0; i < other.data.size(); i++)
		{
			data.push_back(std::pair<unsigned int, void*>(other.data[i].first, new char[other.data[i].first]));
			memcpy(this->data[i].second, other.data[i].second, other.data[i].first);
		}
	}

	TStruct TStruct::operator=(const TStruct& other)
	{
		while (this->data.size())
		{
			pop();
		}

		this->data = vector<std::pair<unsigned int, void*>>();
		for (unsigned int i = 0; i < other.data.size(); i++)
		{
			data.push_back(std::pair<unsigned int, void*>(other.data[i].first, new char[other.data[i].first]));
			memcpy(this->data[i].second, other.data[i].second, other.data[i].first);
		}
		return *this;
	}

	TStruct::TStruct(uint8_t* data, unsigned int dataSize, bool decompress)
	{
		if (data != nullptr)
		{
			uint8_t* dec_data = data;
			unsigned int dec_size = dataSize;
			if (decompress)
			{
				dec_data = new uint8_t[*((unsigned int*)(data))];
				dec_size = *((int*)(data));
				huffman::easyDecode(data + sizeof(int) * 2, dataSize - sizeof(int) * 2, (*(int*)(data + sizeof(int))), dec_data, *((int*)(data)));

				TE_DEBUG_LOG("compression ratio = " + to_string(double(dec_size) / double(dataSize)));
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
		this->data.push_back(std::pair<unsigned int, void*>(size, new char[size]));
		memcpy(this->data.back().second, data, size);
	}

	void TStruct::push(std::string str)
	{
		this->data.push_back(std::pair<unsigned int, void*>(str.length(), new char[str.length()]));
		memcpy(this->data.back().second, str.c_str(), str.length() * sizeof(char));
	}

	void TStruct::pop()
	{
		if (data.size() > 0)
		{
			std::pair<unsigned int, void*> dataBack = data.back();
			delete[] dataBack.second;
			data.pop_back();
		}
	}

	void* TStruct::operator [](unsigned int index)
	{
		if (index < this->data.size())
			return data[index].second;
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
			first.push(second.data[i].first, second.data[i].second);
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
		return data[index].first;
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
			schema += "{size: " + to_string(data[i].first) + ", uint: " + to_string(getFieldValue<unsigned int>(i)) + ", int: " + to_string(getFieldValue<int>(i)) + ", string: \"" + getString(i).substr(0, 32) + "\"}" + ((i - data.size() + 1) ? "," : "");
		schema += "]";
		return schema;
	}
}