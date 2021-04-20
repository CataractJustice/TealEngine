#include <ft2build.h>
#include <freetype/freetype.h>
#include <iostream>
#include "Font.h"

namespace TealEngine 
{
	FontCharacter Font::loadChar(font_char_type character)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (FT_Load_Char((FT_Face)face, character, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}
		FontCharacter& c = characters[character];
		c.texture = Texture(GL_TEXTURE_2D, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
		c.texture.create(FT_Face(face)->glyph->bitmap.width, FT_Face(face)->glyph->bitmap.rows, FT_Face(face)->glyph->bitmap.buffer);
		c.size = glm::ivec2(FT_Face(face)->glyph->bitmap.width, FT_Face(face)->glyph->bitmap.rows);
		c.bearing = glm::ivec2(FT_Face(face)->glyph->bitmap_left, FT_Face(face)->glyph->bitmap_top);
		c.advance = FT_Face(face)->glyph->advance.x;
		return c;
	}
	Font::Font(std::string path)
	{
		FreeTypeUtil::loadFont(path, face);
	}

	Font::Font()
	{
		face = nullptr;
	}

	void Font::setPixelSizes(int height)
	{
		FT_Set_Pixel_Sizes(FT_Face(face), 0, height);
	}

	FontCharacter Font::getCharacter(font_char_type character)
	{
		auto texture = characters.find(character);
		if (texture == characters.cend())
			return loadChar(character);
		else
			return characters[character];
	}
}