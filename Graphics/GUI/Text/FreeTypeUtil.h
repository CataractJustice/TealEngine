#pragma once
#include <string>
struct FT_FaceRec_;
namespace TealEngine
{
	namespace FreeTypeUtil
	{
		int FreeTypeInitialize();

		int loadFont(std::string path, FT_FaceRec_*& face);
	}
}