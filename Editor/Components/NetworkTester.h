#pragma once
#include "GameNode/Component.h"
#include <map>
namespace TealEngine
{
	class NetworkTester : public Component
	{
		private:
		std::map<int, int> peers;
		bool clientConnected;
		char pingCounter;
		char lastPing;
		int port;
		std::string host;
		public:
		virtual void editorUpdate() override;
		virtual void onHostPeerPacket(PeerInfo& peer, const void* data, int size) override;
		virtual void onHostPeerConnect(PeerInfo& peer) override;
		virtual void onHostPeerDisconnect(PeerInfo& peer) override;
		virtual void onClientPacket(const void* data, int size) override;
		virtual void onClientConnect() override;
		virtual void onClientDisconnect() override;
	};
} // namespace TealEngine
