#pragma once
#include <string>
#include "FreeTypeUtil.h"
#include "Graphics/Texture/Texture.h"
#include "libs/glm/vec2.hpp"
#include <map>
namespace TealEngine 
{
	typedef int font_char_type;
	
	struct FontCharacter 
	{
	public:
		Texture texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	class Font 
	{
	private:
		void* face;
		std::map<font_char_type, FontCharacter> characters;

		FontCharacter loadChar(font_char_type character);
	public:
		Font(std::string path);

		Font();

		void setPixelSizes(int height);

		FontCharacter getCharacter(font_char_type character);
	};
}