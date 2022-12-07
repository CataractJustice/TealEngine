#pragma once
#include "../Event.h"
namespace TealEngine
{
	class MouseButtonReleaseEvent : public Event
	{
	public:
		int button, modifier;
		MouseButtonReleaseEvent(int b, int mod) : button(b), modifier(mod) {}
		EVENT_CATEGORY(MOUSE_EVENTS)
		EVENT_TYPE(MOUSE_BUTTON_RELEASED)
	};
}