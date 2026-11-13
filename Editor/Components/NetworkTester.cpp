#include "NetworkTester.h"
#include "Core.h"
namespace TealEngine 
{
	void NetworkTester::editorUpdate() 
	{
		ImGui::Begin("Network Tester");
		if(peers.size() && Core::host.isLive()) 
		{
			ImGui::BulletText("%s", "Peer ping byte list");
			for(auto&[peer, pingByte] : peers) 
			{
				ImGui::Text("%d : %d", peer, pingByte);
				std::string buttonLabel = "ping " + std::to_string(peer);
				if(ImGui::Button(buttonLabel.c_str())) 
				{
					PeerInfo p;
					p.id = peer;
					pingCounter++;
					Core::host.send(this->getNetworkId(), p, &pingCounter, 1);
				}
			}
		}
		
		ImGui::Text("Last ping: %d", lastPing);

		ImGui::BulletText("%s: %d", "Ping counter", pingCounter);
		
		ImGui::InputText("Host address", &host);
		ImGui::InputInt("Host port", &port);

		if(!Core::host.isLive() && ImGui::Button("Host")) 
		{
			Core::host.host(port, 100);
		}
		if(!Core::client.isLive() && ImGui::Button("Connect")) 
		{
			Core::client.connect(host, port);
		}
		if((Core::client.isLive() || Core::host.isLive()) && ImGui::Button("Ping")) 
		{
			if(Core::client.isLive()) 
			{
				pingCounter++;
				Core::client.send(this->getNetworkId(), &pingCounter, 1);
			}
			if(Core::host.isLive()) 
			{
				pingCounter++;
				Core::host.broadcast(this->getNetworkId(), &pingCounter, 1);
			}
		}
		
		ImGui::End();
	}

	void NetworkTester::onClientConnect() 
	{
		clientConnected = true;
	}
	void NetworkTester::onClientDisconnect() 
	{
		clientConnected = false;
	}
	void NetworkTester::onClientPacket(const void* data, int size) 
	{
		if(size) 
		{
			this->lastPing = *(char*)data;
		}
	}
	
	void NetworkTester::onHostPeerConnect(PeerInfo& peer) 
	{
		peers[peer.id] = 0;
	}
	void NetworkTester::onHostPeerDisconnect(PeerInfo& peer) 
	{
		peers.erase(peer.id);
	}
	void NetworkTester::onHostPeerPacket(PeerInfo& peer, const void* data, int size) 
	{
		if(size) 
		{
			peers[peer.id] = *(char*)data;
		}
	}
}