#include "Text.h"
#include "Resources.h"
namespace TealEngine
{
	void Text::render()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec2 cursor = glm::vec2(getTransform().getParamRealValue(GUI_X_OFFSET), getTransform().getParamRealValue(GUI_Y_OFFSET));
		float baseline = cursor.y;
		for (wchar_t c : text)
		{
			if (c == L'\n')
			{
				cursor = glm::vec2(getTransform().getParamRealValue(GUI_X_OFFSET), cursor.y - (float)font->getPixelSizes() / Graphics::window->getWindowHeight() * scale);
				continue;
			}

			FontCharacter character = font->getCharacter(c);
			vec2 charPos;
			vec2 charScale;
			charPos = cursor + vec2(character.bearing.x * scale, -(character.size.y - character.bearing.y) * scale) / Graphics::window->getScreenResolution();
			charScale = vec2(character.size) * scale / Graphics::window->getScreenResolution();
			ShaderProgram& textShader = Resources::getShader("basic_text");
			textShader.setTexture("tex", character.texture.id());
			Render::renderShader(&textShader, charPos + charScale / 4.0f, charScale / 2.0f);
			cursor.x += float(character.advance >> 6) * scale / Graphics::window->getScreenResolution().x;
		}
		glDisable(GL_BLEND);
	}
}