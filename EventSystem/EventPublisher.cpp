#include "EventPublisher.h"
namespace TealEngine
{
	void EventPublisher::subscribe(EventListener* el) 
	{
		this->subscribers[el->getId()] = el;
	};
	void EventPublisher::unsubscribe(EventListener* el) 
	{
		this->subscribers.erase(el->getId());
	};
	void EventPublisher::operator()(Event* e) 
	{
		for (auto el = this->subscribers.begin(); el != subscribers.end();) 
		{
			if ((*el).second != nullptr)
			{
				(*el).second->operator()(e);
				++el;
			}
			else 
			{
				this->subscribers.erase(el++);
			}
		}
	};
}