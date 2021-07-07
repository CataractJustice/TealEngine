#pragma once
#include <initializer_list>
#include <string>
#include <map>
class PreparedTStructSchema
{
private:
	std::map<std::string, int> dictionary;
public:
	PreparedTStructSchema(std::initializer_list<std::string> list)
	{
		int i = 0;
		for (std::string name : list)
		{
			dictionary.insert({ name, i++ });
		}
	}

	int getLength() 
	{
		return dictionary.size();
	}

	int getIndexOf(std::string fieldName) 
	{
		auto iter = dictionary.find(fieldName);
		if (iter == dictionary.cend())
			return -1;
		return iter->second;
	}
};