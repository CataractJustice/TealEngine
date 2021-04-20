#include "Graphics/Renderer/DefferedRenderer.h";
#include "EventSystem/EventSystem.h"
#include "TClient.h"
#include "Server.h"
#include "GameNode/Entity/Entity.h"
#include "GameNode/Prefab/PrefabFactory.h"
#define CONNECTION_TIMEOUT 10000
namespace TealEngine 
{
	class ClientNode : public GameNode
	{
	private:
		TClient socket;
		std::map<unsigned int, Entity*> entities;
		EventListener messageDispatcher;
		void dispatchEvent(Event* e);

		void addEntity(Entity* e, unsigned int id);

	public:
		void update();

		ClientNode(std::string ip, unsigned int port);

		void sendEntityData(Entity* entity, TPacket data, enet_uint32 packetFlags = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
	};
}