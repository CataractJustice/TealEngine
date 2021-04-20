#pragma once
#include "Event.h"
#include <functional>
#include <unordered_set>
namespace TealEngine
{
	class EventPublisher;
	#define eventListenerFunc std::function<void(Event* e)>
	
	#define eventListenerBind(method, object) eventListenerFunc(std::bind(method, object, std::placeholders::_1))

	class EventListener
	{
	private:
		std::unordered_set<EventPublisher*> subscribtions;
		unsigned int id;
	protected:
		eventListenerFunc executable;
	public:
		EventListener();
		~EventListener();
		unsigned int getId();
		void subscribeTo(EventPublisher* ep);
		EventListener& operator=(const EventListener el);
		EventListener& operator=(const eventListenerFunc ex);
		void operator()(Event* e);
	};
}