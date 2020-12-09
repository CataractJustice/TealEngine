#pragma once
#include "../Event.h"
#include "../../Network/PeerData.h"
namespace TealEngine
{
	class PeerDisconnectEvent : public Event
	{
	public:
		PeerData peerData;
		PeerDisconnectEvent(PeerData peerData) : peerData(peerData)
		{}
		EVENT_CATEGORY(NETWORK_EVENTS)
			EVENT_TYPE(PEER_DISCONNECTED)
	};
}