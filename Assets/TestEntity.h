#pragma once
#include "../GameNode/GameNode.h"
#include "../GameNode/Entity/Entity.h"

namespace TealEngine 
{
	class TestEntity : public Entity 
	{
	public:
		bool isVisibleForPeer(PeerData peer) 
		{
			return true;
		}



		TestEntity(unsigned int id = 0) 
		{
			Entity::Entity(id);
		}
	};
}