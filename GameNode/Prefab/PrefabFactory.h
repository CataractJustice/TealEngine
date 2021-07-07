#pragma once
#include "GameNode/Entity/Entity.h"
#include "System/TStruct.h"
#include "System/Debug.h"


namespace TealEngine 
{
class IPrefab;
	namespace PrefabFactory
	{
		Entity* createPrefabInstance(std::string prefabName, bool serverSide);
		IPrefab* getPrefab(std::string name);
		void addPrefab(std::string prefabName, IPrefab* prefab);
	};
}