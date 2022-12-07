#include "getDirectoryList.h"
#include <experimental/filesystem>
using namespace std;

vector<string> getDirectoryList(std::string path) 
{
	vector<string> result;
	for (const auto& entry : experimental::filesystem::directory_iterator(path))
		result.push_back(entry.path().filename().string());
	return result;
}