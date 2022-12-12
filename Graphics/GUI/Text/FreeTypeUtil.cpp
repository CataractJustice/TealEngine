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
			return 0;
		}

		int loadFont(std::string path, FT_Face& face)
		{
			int e;
			FT_Face a;
			if (e = FT_New_Face(ft, path.c_str(), 0, &a))
			{
				std::cout << "ERROR::FREETYPE: Failed to load font from \"" << path << "\"" << "error code: " << e << std::endl;
				return e;
			}
			face = a;
			return 0;
		}
	}
}