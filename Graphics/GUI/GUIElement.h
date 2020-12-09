#pragma once
#include "../../GameNode/GameNode.h"
#include "GUITransform.h"
namespace TealEngine 
{
	class GUIElement : public GameNode
	{
	private:
		GUITransform transform;
	public:
		GUIElement() 
		{
			
		}
	};
}