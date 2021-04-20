#pragma once
#include <string>
namespace TealEngine
{
	namespace FreeTypeUtil
	{
		int FreeTypeInitialize();

		int loadFont(std::string path, void*& face);
	}
}