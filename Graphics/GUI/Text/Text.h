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
		Font* font;
		float scale;

		void render();
	};
}