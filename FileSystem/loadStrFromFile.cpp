#include "loadStrFromFile.h"
#include "System/Debug.h"
#include<fstream>
#include<sstream>
#include <iostream>
std::string loadStrFromFile(std::filesystem::path path, int* errorcode)
{
	std::ifstream file;
	file.open(path);
	if (!file) 
	{
		if(errorcode)
			*errorcode = 1;
	}
	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
}