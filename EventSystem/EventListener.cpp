#pragma once
#include "EventListener.h"
#include "EventPublisher.h"
namespace TealEngine
{
	unsigned int lastId = 0;

	EventListener::EventListener()
	{
		id = lastId;
		lastId++;
	}

	EventListener::~EventListener()
	{
		for (EventPublisher* ep : subscribtions)
			ep->unsubscribe(this);
	}

	unsigned int EventListener::getId() { return id; }

	//does not actualy subscribe to event publisher
	void EventListener::subscribeTo(EventPublisher* ep) 
	{
		this->subscribtions.insert(ep);
	}

	EventListener& EventListener::operator=(const EventListener el)
	{
		this->executable = el.executable;
		return *this;
	}

	EventListener& EventListener::operator=(const  std::function<void(Event* e)> ex)
	{
		this->executable = ex;
		return *this;
	}

	void EventListener::operator()(Event* e)
	{
		executable(e);
	}

}