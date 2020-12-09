#include "loadStrFromFile.h"
#include <iostream>
std::string loadStrFromFile(std::string path)
{
	std::ifstream file;
	file.open(path);
	if (!file) 
	{
		std::cout << "loadStrFromFile() ERROR: file not found.\n";
		return "";
	}
	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
}