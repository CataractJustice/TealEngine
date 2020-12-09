#pragma once
#include "../Event.h"
#include "../../Network/PeerData.h"
namespace TealEngine
{
	class PeerConnectEvent : public Event
	{
	public:
		PeerData peerData;
		PeerConnectEvent(PeerData peerData) : peerData(peerData)
		{}
		EVENT_CATEGORY(NETWORK_EVENTS)
			EVENT_TYPE(PEER_CONNECTED)
	};
}