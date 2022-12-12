#include "loadStrFromFile.h"
#include "System/Debug.h"
#include<fstream>
#include<sstream>
#include <iostream>
std::string loadStrFromFile(std::string path, int* errorcode)
{
	std::ifstream file;
	file.open(path);
	if (!file) 
	{
		TE_DEBUG_ERROR("File " + path + " not found.\n");
		if(errorcode)
			*errorcode = 1;
	}
	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
}