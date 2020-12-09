#pragma once
#include "../../GameNode/GameNode.h"
namespace TealEngine
{
	class ChildNodeRemoveEvent : public Event
	{
	public:
		GameNode* node;
		ChildNodeRemoveEvent(GameNode* node)
		{
			this->node = node;
		}

		EVENT_TYPE(CHILD_REMOVED);
		EVENT_CATEGORY(GAME_NODE_EVENTS);
	};
}