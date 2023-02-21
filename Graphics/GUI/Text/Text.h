#pragma once
#include "GameNode/Component.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Font.h"
namespace TealEngine 
{
	class FrameBuffer;
	class Text : public Component
	{
	private:
		Texture texture;
		glm::vec4 color;
		glm::vec4 rect;
		std::string text;
		float scale;
		//renders text into texture
		void refresh();
		static FrameBuffer TextRenderBuffer;
	public:
		glm::vec2 screenPosition;
		Font* font;
		Text();
		const std::string& getText();
		void setText(const std::string& text);
		float getScale();
		void setScale(float scale);
		const glm::vec4& getColor();
		void setColor(const glm::vec4& color);
		//displays text
		void GUIrender() override;
	};
}