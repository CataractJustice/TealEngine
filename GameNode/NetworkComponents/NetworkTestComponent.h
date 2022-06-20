#pragma once
#include "GameNode/Component.h"

namespace TealEngine
{
	class NetworkTestComponent : public Component
	{
	public:
		void onAttach()
		{
			TE_DEBUG_LOG("NetworkTestComponent attached to a node.");
		}

		void onMessageReceive(TPacket& packet) 
		{
			TE_DEBUG_LOG("NetworkTestComponent received a message that says: \"" + packet.getString(0) + "\"");
		}
	private:
	};
}