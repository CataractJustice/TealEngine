#include "Graphics/Renderer/DefferedRenderer.h";
#include "EventSystem/EventSystem.h"
#include "TClient.h"
#include "Server.h"
#include "GameNode/Entity/Entity.h"
#include "GameNode/Prefab/PrefabFactory.h"
#include "GameNode/Component.h"
#define CONNECTION_TIMEOUT 10000
namespace TealEngine 
{
	class ClientComponent : public Component
	{
	private:
		TClient socket;
		std::map<unsigned int, Entity*> entities;
		EventListener messageDispatcher;
		void dispatchEvent(Event* e);

		void addEntity(Entity* e, unsigned int id);

	public:
		void update();

		ClientComponent(std::string ip, unsigned int port);

		void sendEntityData(Entity* entity, TPacket data, unsigned int packetFlags = 8);
	};
}