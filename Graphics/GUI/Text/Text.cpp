#include "Text.h"
#include "Resources.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "Math/map.h"
namespace TealEngine
{

	Font* font;
	const wstring& Text::getText() {
		return this->text;
	}

	void Text::setText(const wstring& text) {
		if (!text.compare(this->text)) return;
		this->text = text;
		this->refresh();
	}

	float Text::getScale() {
		return this->scale;
	}

	void Text::setScale(float scale) {
		this->scale = scale;
	}

	const glm::vec4& Text::getColor() {
		return this->color;
	}

	void Text::setColor(const glm::vec4& color) {
		this->color = color;
	}
	//displays text
	void Text::GUIrender() {
		if (rect.w == rect.y)
			return;
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec4 renderRect = rect / (rect.w - rect.y) * scale;
		Render::renderTexture(texture.id(), 
			this->screenPosition,
			glm::vec2(renderRect.z - renderRect.x, renderRect.w - renderRect.y)
		);
		glDisable(GL_BLEND);
	}

	void Text::refresh() {
		rect = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		std::vector<glm::vec4>  charRects;
		charRects.reserve(this->text.length());
		glm::vec2 cursor(0.0f);
		for (wchar_t c : text)
		{
			if (c == L'\n')
			{
				cursor = glm::vec2(0.0f, cursor.y + (float)font->getPixelSizes() * 2.0f);
				continue;
			}

			FontCharacter character = font->getCharacter(c);
			vec2 charPos = cursor + vec2(character.bearing.x, -character.size.y - character.bearing.y);
			glm::vec4 charRect = glm::vec4(charPos, glm::vec2(character.size));
			rect = 
				glm::vec4(
					glm::min(
						glm::vec2(charRect.x, charRect.y), 
						glm::vec2(rect.x, rect.y)
					), 
					glm::max(
						glm::vec2(charRect.x + charRect.z, charRect.y + charRect.w), 
						glm::vec2(rect.z, rect.w)
					)
				);

			cursor.x += float(character.advance >> 5);
			charRects.push_back(charRect);
		}
		
		texture = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

		texture.setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		texture.setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		texture.create(rect.z - rect.x, rect.w - rect.y);
		TextRenderBuffer.resize(rect.z - rect.x, rect.w - rect.y);
		TextRenderBuffer.attachTexture(texture.id(), 0);
		TextRenderBuffer.enable(0);
		TextRenderBuffer.attachTexture(texture.id(), 1);
		TextRenderBuffer.enable(1);
		TextRenderBuffer.apply();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		int i = 0;
		for (vec4& charRect : charRects) {
			if (text[i] == '\n') {
				i++;
			}
			charRect = Math::map(glm::vec4(rect.x, rect.y, 0.0f, 0.0f), glm::vec4(rect.z, rect.w, rect.z - rect.x, rect.w - rect.y), glm::vec4(-1.0f, 1.0f, 0.0f, 0.0f), glm::vec4(1.0f, -1.0f, 1.0f, 1.0f), charRect);
			Resources::getShader("basic_text").setTexture("tex", font->getCharacter(text[i]).texture.id());
			i++;
			Render::renderShader(&Resources::getShader("basic_text"),  glm::vec2(charRect.x + charRect.z * 2.0f, charRect.y - charRect.w * 2.0f), glm::vec2(charRect.z, charRect.w) * 2.0f);
		}
		glDisable(GL_BLEND);
		TextRenderBuffer.unbind();
	}

	FrameBuffer Text::TextRenderBuffer;
}