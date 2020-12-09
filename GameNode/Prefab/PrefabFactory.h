#pragma once
#include "../Entity/Entity.h"
#include "../../System/TStruct.h"
#include "../../System/Debug.h"

namespace TealEngine 
{
	#define PrefabCreatingFunction std::function<Entity* (bool serverSide, TStruct*)>

	static class PrefabFactory
	{
	private:
		PrefabFactory() {};
		static std::map<std::string, PrefabCreatingFunction> prefabs;
	public:
		static Entity* createPrefabInstance(std::string prefabName, bool serverSide, TStruct* params)
		{
			if (prefabs.find(prefabName) == prefabs.cend())
			{
				TE_DEBUG_ERROR("There is no \"" + prefabName + "\" prefab.");
			}
			else
			{
				Entity* prefab = prefabs[prefabName](serverSide,params);
				prefab->setEntityTypeName(prefabName);
				return prefab;
			}
		}

		static void addPrefab(std::string prefabName, PrefabCreatingFunction prefabCreationFunc)
		{
			if (prefabs.find(prefabName) != prefabs.cend()) 
			{
				TE_DEBUG_WARNING("Prefab \"" + prefabName + "\" already existed but get overriden.");
			}
			prefabs[prefabName] = prefabCreationFunc;
		}

		static void init();
	};
}