#include "isDirectory.h"
#include<experimental/filesystem>

bool isDirectory(std::string path) {
	return std::experimental::filesystem::is_directory(path);
}