#include "saveStrToFile.h"
#include<fstream>

namespace TealEngine 
{
	void saveStrToFile(std::string path, std::string str) 
	{
		std::ofstream file;
		file.open(path, 'w');
		file.write(str.c_str(), str.length());
	}
}