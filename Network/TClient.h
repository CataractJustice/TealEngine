#pragma once
#include "EventSystem/EventPublisher.h"
#include "EventSystem/NetworkEvents/NetworkEvents.h"
#include "PeerData.h"
#include "System/Debug.h"
#include "enet/enet.h"
#include <queue>
#include <thread>
namespace TealEngine 
{
	class TClient
	{
	private:
		void* enetHost;
		void* enetAddress;
		std::queue<Event*> eventsQueue;
		std::thread* serviceThread;

		void serviceThreadMethod();
		void startServiceThread();

	public:
		bool connect(unsigned int timeout, std::string ip, unsigned int port);
		void disconnect();
		void pollEvents();
		void send(TPacket tpacket, unsigned int packetFlags);
		EventPublisher onConnect;
		EventPublisher onRecive;
		EventPublisher onDisconnect;
	};
}

