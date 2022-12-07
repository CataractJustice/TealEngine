#pragma once
#include "../../GameNode/GameNode.h"
namespace TealEngine 
{
	class ChildNodeAddEvent : public Event
	{
	public:
		GameNode* node;
		ChildNodeAddEvent(GameNode* node)
		{
			this->node = node;
		}

		EVENT_TYPE(CHILD_ADDED);
		EVENT_CATEGORY(GAME_NODE_EVENTS);
	};
}