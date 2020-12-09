#pragma once
#include "../Event.h"
namespace TealEngine
{
	class MouseMoveEvent : public Event
	{
	public:
		double xpos, ypos, xdelta, ydelta;
		MouseMoveEvent(double x, double y, double dx, double dy) : xpos(x), ypos(y), xdelta(dx), ydelta(dy) {}
		EVENT_CATEGORY(MOUSE_EVENTS)
		EVENT_TYPE(MOUSE_BUTTON_PRESSED)
	};
}