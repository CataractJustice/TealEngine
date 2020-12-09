#pragma once
#include "../Event.h"

namespace TealEngine
{
	class WindowResizeEvent : public Event
	{
	public:
		unsigned int width, height;
		WindowResizeEvent(unsigned int w, unsigned int h) : width(w), height(h) {}
		EVENT_CATEGORY(WINDOW_EVENTS);
		EVENT_TYPE(WINDOW_RESIZE);
	};
}