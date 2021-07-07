#include "PrefabFactory.h"
#include "IPrefab.h"
namespace TealEngine 
{
	std::map<std::string, IPrefab*> prefabs = std::map<std::string, IPrefab*>();

	Entity* PrefabFactory::createPrefabInstance(std::string prefabName, bool serverSide)
	{
		if (prefabs.find(prefabName) == prefabs.cend())
		{
			TE_DEBUG_ERROR("There is no \"" + prefabName + "\" prefab.");
		}
		else
		{
			Entity* prefab =  (*prefabs[prefabName])(serverSide);
			prefab->setEntityTypeName(prefabName);
			return prefab;
		}
	}

	IPrefab* PrefabFactory::getPrefab(std::string name) 
	{
		return prefabs[name];
	}

	void PrefabFactory::addPrefab(std::string prefabName, IPrefab* prefabCreationFunc)
	{
		if (prefabs.find(prefabName) != prefabs.cend())
		{
			TE_DEBUG_WARNING("Prefab \"" + prefabName + "\" already existed but get overriden.");
		}
		prefabs[prefabName] = prefabCreationFunc;
	}
}