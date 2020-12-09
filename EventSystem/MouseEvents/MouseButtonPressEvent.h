#pragma once
#include "../Event.h"
namespace TealEngine
{
	class MouseButtonPressEvent : public Event
	{
	public:
		int button, modifier;
		MouseButtonPressEvent(int b, int mod) : button(b), modifier(mod) {}
		EVENT_CATEGORY(MOUSE_EVENTS)
		EVENT_TYPE(MOUSE_BUTTON_PRESSED)
	};
}