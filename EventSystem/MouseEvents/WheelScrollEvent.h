#pragma once
#include "../Event.h"
namespace TealEngine
{
	class WheelScrollEvent : public Event
	{
	public:
		double xscroll, yscroll;
		WheelScrollEvent(double x, double y) : xscroll(x), yscroll(y) {}
		EVENT_CATEGORY(MOUSE_EVENTS)
		EVENT_TYPE(MWHEEL_SCROLLED)
	};
}