#pragma once
#include "Graphics/GUI/GUIElement.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Font.h"
namespace TealEngine 
{
	class FrameBuffer;
	class Text : public GUIElement
	{
	private:
		Texture texture;
		glm::vec4 color;
		glm::vec4 rect;
		wstring text;
		float scale;
		//renders text into texture
		void refresh();
		static FrameBuffer TextRenderBuffer;
	public:
		Font* font;
		const wstring& getText();
		void setText(const wstring& text);
		float getScale();
		void setScale(float scale);
		const glm::vec4& getColor();
		void setColor(const glm::vec4& color);
		//displays text
		void render();
	};
}