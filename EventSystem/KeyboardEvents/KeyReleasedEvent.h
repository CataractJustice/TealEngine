#pragma once
#include "../Event.h"

namespace TealEngine
{
	class KeyReleaseEvent : public Event
	{
	public:
		int key, scancode, modifier;
		KeyReleaseEvent(int k, int s, int mod) : key(k), scancode(s), modifier(mod) {}
		EVENT_CATEGORY(KEYBOARD_EVENTS)
		EVENT_TYPE(KEY_RELEASED)
	};
}