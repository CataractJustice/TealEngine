#include "isDirectory.h"
#include<filesystem>

bool isDirectory(std::string path) {
	return std::filesystem::is_directory(path);
}