#pragma once
#include "../Event.h"
namespace TealEngine
{
	class ValueChangeEvent : public Event
	{
	public:
		ValueChangeEvent() {};
		EVENT_CATEGORY(MISC_EVENTS)
			EVENT_TYPE(VALUE_CHANGED)
	};
}