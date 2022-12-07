#include "getFilesInDir.h"
#include "getDirectoryList.h"
#include "isDirectory.h"
std::vector<std::string> getFilesInDir(std::string path, bool subdirectories) 
{
	std::vector<std::string> items = getDirectoryList(path);
	std::vector<std::string> files;
	for (unsigned int i = 0; i < items.size(); i++) 
	{
		if (isDirectory(path + "/" + items[i]) && subdirectories)
		{
			std::vector<std::string> subfiles = getFilesInDir(path + "/" + items[i], true);
			for (unsigned int j = 0; j < subfiles.size(); j++) 
			{
				files.push_back(items[i] + "/" + subfiles[j]);
			}
		}
		else 
		{
			files.push_back(items[i]);
		}
	}

	return files;
}