#pragma once
#include <string>
#include <map>
#include <array>
namespace TealEngine 
{
	class SceneNode;
	struct HostData;
	struct ClientData;
	typedef std::array<unsigned long long, 2> NetworkId;
	struct PeerInfo 
	{
		int id;
	};
	class Host
	{
		HostData* hostImplData;
		int port;
		int peerIdCounter;
		bool live;
		std::map<int, void*> peers;
		public:
		Host();
		~Host();
		inline bool isLive() const { return live; };
		void host(int port, int connMax);
		void close();
		void send(const NetworkId& toComponentId, PeerInfo& peer, const void* bytes, int size) const;
		void broadcast(const NetworkId& toComponentId, const void* bytes, int size) const;
		void update(SceneNode* scene);
	};

	class Client 
	{
		private:
		ClientData* clientImplData;
		std::string host;
		bool live;
		bool connected;
		public:
		Client();
		~Client();
		void connect(const std::string& host, int port);
		void send(const NetworkId& toComponentId, const void* bytes, int size) const;
		void update(SceneNode* scene);
		void close();
		const std::string& getHostStr() const { return host; }
		inline bool isLive() const { return live; }
		inline bool isConnected() const { return connected; }
	};
}