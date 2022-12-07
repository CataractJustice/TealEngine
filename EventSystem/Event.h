#pragma once
#include <string>

namespace TealEngine 
{
	enum EventType 
	{
		MISC,
		WINDOW_RESIZE,
		KEY_PRESSED,
		KEY_REPEAT,
		KEY_RELEASED,
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MWHEEL_SCROLLED,
		INVOKE,
		CHILD_ADDED,
		CHILD_REMOVED,
		VALUE_CHANGED,
		MSG_RECIVED,
		PEER_CONNECTED,
		PEER_DISCONNECTED,
		EVENT_TYPES_LENGTH
	};

	enum EventCategory
	{
		WINDOW_EVENTS = 1,
		KEYBOARD_EVENTS = 1 << 2,
		MOUSE_EVENTS = 1 << 3,
		PHYSIC_EVENTS = 1 << 4,
		GAME_NODE_EVENTS = 1 << 5,
		NETWORK_EVENTS = 1 << 6,
		MISC_EVENTS = 1 << 7
	};

#define EVENT_TYPE(x) virtual EventType getType() const {return x;} \
virtual std::string getName() const {return std::string(#x);}
#define EVENT_CATEGORY(x) virtual int getCategoryFlags() const {return x;}

	class Event 
	{
	private:

	public:
		virtual EventType getType() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string getName() const = 0;
		
		inline bool isInCategory(EventCategory category) 
		{
			return category & getCategoryFlags();
		}
	};
}