#pragma once
#include "FreeTypeUtil.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
namespace TealEngine
{
	namespace FreeTypeUtil
	{
		FT_Library ft;

		int FreeTypeInitialize()
		{
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
				return -1;
			}
		}

		int loadFont(std::string path, void*&face)
		{
			int e;
			if (e = FT_New_Face(ft, path.c_str(), 0, (FT_Face*)face))
			{
				std::cout << "ERROR::FREETYPE: Failed to load font from \"" << path << "\"" << "error code: " << e << std::endl;
				return -1;
			}
			return 0;
		}
	}
}