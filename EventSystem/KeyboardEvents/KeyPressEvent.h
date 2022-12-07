#pragma once
#include "../Event.h"

namespace TealEngine
{
	class KeyPressEvent : public Event
	{
	public:
		int key, scancode, modifier;
		KeyPressEvent(int k, int s, int mod) : key(k), scancode(s), modifier(mod) {}
		EVENT_CATEGORY(KEYBOARD_EVENTS)
		EVENT_TYPE(KEY_PRESSED)
	};
}