#pragma once
#include "Graphics/GUI/GUIElement.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Font.h"
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
			glm::vec2 cursor = glm::vec2(getTransform().getParamRealValue(GUI_X_OFFSET), getTransform().getParamRealValue(GUI_Y_OFFSET));
			float baseline = cursor.y;
			for (char c : text)
			{
				FontCharacter character = font.getCharacter(c);
				vec2 charPos;
				vec2 charScale;
				charPos = cursor + vec2(character.bearing.x * scale, -(character.size.y - character.bearing.y) * scale) / Graphics::Window::getResolution();
				charScale = vec2(character.size) * scale / Graphics::Window::getResolution();
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				ShaderProgram textShader = Resources::getShader("basic_text");
				textShader.setTexture("tex", character.texture.id());
				textShader.setUniform("scale", charScale/2.0f);
				textShader.setUniform("offset", charPos+charScale/4.0f);
				Render::renderShader(&textShader);
				cursor.x += float(character.advance >> 6) * scale / Graphics::Window::getResolution().x;
			}
		}
	};
}