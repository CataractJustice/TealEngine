#pragma once
#include <filesystem>
#include <vector>
#include <fstream>

inline std::vector<char> loadBytesFromFile(std::filesystem::path path)
{
	std::ifstream ifs(path, std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	if (pos == 0) {
		return std::vector<char>{};
	}

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}
