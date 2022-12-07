#pragma once
#include <map>
#include "EventListener.h"
namespace TealEngine 
{
	class EventPublisher 
	{
	private:
		std::map<unsigned int, EventListener*> subscribers;
	public:
		void subscribe(EventListener* el);
		void unsubscribe(EventListener* el);
		void operator()(Event* e);
	};
}