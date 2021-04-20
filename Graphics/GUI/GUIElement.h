#pragma once
#include "../../GameNode/GameNode.h"
#include "GUITransform.h"
#include "DefaultTags.h"
namespace TealEngine 
{
	class GUIElement : public GameNode
	{
	private:
		GUITransform transform;
	public:
		GUIElement() 
		{
			addTag(GUI_ELEMENT_TAG);
		}

		void renderAll() 
		{
			auto GUIChilds = findNodesByTag(GUI_ELEMENT_TAG);
			for (auto node : GUIChilds)
			{
				((GUIElement*)node)->render();
			}
			render();
		}

		virtual void render() {}

		GUITransform& getTransform() { return transform; }
	};
}