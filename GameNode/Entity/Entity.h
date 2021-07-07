#pragma once
#include <map>
#include <string>
#include "../GameNode3D.h"
#include "../../Network/PeerData.h"
using namespace std;
namespace TealEngine 
{
	class ClientComponent;
	class Server;
	class Entity : public GameNode3D
	{
	private:
		unsigned int entityID;
		std::string entityTypeName;
	protected:
		Server* server;
		ClientComponent* client;
	public:

		std::string getEntityTypeName();
		void setEntityTypeName(std::string typeName);

		void setServer(Server* server);
		void setClient(ClientComponent* node);
		
		unsigned int getEntityID();
		void setEntityID(unsigned int id);

		virtual bool isVisibleForPeer(PeerData peer);
		Entity(unsigned int id = 0, std::string typeName = "");
	};
}