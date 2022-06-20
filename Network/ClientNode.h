#include "Graphics/Renderer/DefferedRenderer.h";
#include "EventSystem/EventSystem.h"
#include "TClient.h"
#include "Server.h"
#include "GameNode/Component.h"
#define CONNECTION_TIMEOUT 10000
namespace TealEngine 
{
	class ClientComponent : public Component
	{
	private:
		TClient socket;
		std::map<NodeNetworkIDType, GameNode*> nodesIdMap;
		std::map<ComponentNetworkIDType, Component*> componentsIdMap;
		EventListener messageDispatcher;
		void dispatchEvent(Event* e);

	public:
		void update();

		ClientComponent(std::string ip, unsigned int port);

		//void sendToComponent(Component* component, TPacket& packet);
		//void sendToNode(GameNode* node, TPacket& packet);
	};
}