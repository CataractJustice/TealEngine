#pragma once
#include "../Event.h"
namespace TealEngine
{
	class InvokeEvent : public Event
	{
	public:
		
		InvokeEvent() {};
		EVENT_CATEGORY(MISC_EVENTS)
		EVENT_TYPE(INVOKE)
	};
}