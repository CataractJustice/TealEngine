#include <string>
#include <iostream>
#include <filesystem>
#include "getDirectoryList.h"

std::vector<std::string> getDirectoryList(std::string path)
{
	std::vector<std::string> dirs;
    for (const auto & entry : std::filesystem::directory_iterator(path))
    	dirs.push_back(entry.path().filename());
	return dirs;
}