#include "Random.h"
#include <fstream>

namespace TealEngine
{
	std::vector<char> randomBytes(int length) 
	{
		std::ifstream randFile("/dev/urandom");
		std::vector<char> bytes;
		bytes.resize(length, 0);
		randFile.read(bytes.data(), length);
		return bytes;
	}
	void randomBytes(char* buffer, int length) 
	{
		std::ifstream randFile("/dev/urandom");
		randFile.read(buffer, length);
	}
} // namespace TealEngine
