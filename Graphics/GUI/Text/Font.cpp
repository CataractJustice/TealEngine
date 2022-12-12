#include <ft2build.h>
#include <iostream>
#include "Font.h"

namespace TealEngine
{
	FontCharacter Font::loadChar(font_char_type character)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		FT_Error e = FT_Load_Char(face, character, FT_LOAD_RENDER);
		if (e)
		{
			std::cout << e;
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << FT_Error_String(e) << std::endl;
		}
		FontCharacter& c = characters[character];
		c.texture = Texture(GL_TEXTURE_2D, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
		c.texture.create(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
		c.texture.setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		c.texture.setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		c.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		c.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		c.advance = face->glyph->advance.x;
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
		FT_Set_Pixel_Sizes(face, 0, height);
		this->height = height;
	}

	int Font::getPixelSizes()
	{
		return height;
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
