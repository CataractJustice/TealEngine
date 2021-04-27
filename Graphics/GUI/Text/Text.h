#pragma once
#include "Graphics/GUI/GUIElement.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Font.h"
#include "Resources.h"
namespace TealEngine 
{
	class Text : public GUIElement
	{
	private:
	public:
		wstring text;
		Font font;
		float scale;

		void render()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glm::vec2 cursor = glm::vec2(getTransform().getParamRealValue(GUI_X_OFFSET), getTransform().getParamRealValue(GUI_Y_OFFSET));
			float baseline = cursor.y;
			for (char c : text)
			{
				FontCharacter character = font.getCharacter(c);
				vec2 charPos;
				vec2 charScale;
				charPos = cursor + vec2(character.bearing.x * scale, -(character.size.y - character.bearing.y) * scale) / Graphics::window->getResolution();
				charScale = vec2(character.size) * scale / Graphics::window->getResolution();
				ShaderProgram& textShader = Resources::getShader("basic_text");
				textShader.setTexture("tex", character.texture.id());
				Render::renderShader(&textShader, charPos + charScale / 4.0f, charScale / 2.0f);
				cursor.x += float(character.advance >> 6) * scale / Graphics::window->getResolution().x;
			}
			glDisable(GL_BLEND);
		}
	};
}