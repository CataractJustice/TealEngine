#pragma once
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
namespace TealEngine
{
	namespace FreeTypeUtil
	{
		int FreeTypeInitialize();

		int loadFont(std::string path, FT_Face& face);
	}
}