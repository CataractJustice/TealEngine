#pragma once
#include "Graphics/Texture/Texture.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "GUIElement.h"
#include <list>
namespace TealEngine 
{
	class GUIRenderer
	{
	private:
		std::list<GUIElement*> elements;
		FrameBuffer fb;
	public:
		void render(Texture renderTexture) 
		{
			fb.resize(Graphics::window->getScreenWidth(), Graphics::window->getScreenHeight());
			fb.bind();
			fb.attachTexture(renderTexture.id(), 0);
			fb.enable(0);
			fb.apply();
			for (GUIElement* element : elements)
				if (GameNode::isNodeExist(element))
					element->renderAll();
		}

		void render() 
		{
			for (GUIElement* element : elements)
				if (GameNode::isNodeExist(element))
					element->renderAll();
		}

		void push(GUIElement* element) 
		{
			elements.push_back(element);
		}
	};
}