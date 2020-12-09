#pragma once
#include "../Event.h"
#include "../../Network/PeerData.h"
#include "../../Network/TPacket.h"
namespace TealEngine
{
	class MsgReciveEvent : public Event
	{
	public:
		PeerData peerData;
		TPacket message;
		MsgReciveEvent(PeerData peerData, TPacket message) 
		{
			this->peerData = peerData;
			this->message = message;
		}
		EVENT_CATEGORY(NETWORK_EVENTS)
		EVENT_TYPE(MSG_RECIVED)
	};
}