#pragma once
#include<string>
#include <filesystem>
std::string loadStrFromFile(std::filesystem::path path, int* errorcode = nullptr);