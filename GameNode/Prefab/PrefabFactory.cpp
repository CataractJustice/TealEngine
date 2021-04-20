#include "PrefabFactory.h"

#include "../../Assets/TestCubePrefab.h"
#include "../../Assets/Voxel/VoxelWorldPrefab.h"
namespace TealEngine 
{
	std::map<std::string, IPrefab*> prefabs = std::map<std::string, IPrefab*>();

	Entity* PrefabFactory::createPrefabInstance(std::string prefabName, bool serverSide, TStruct* params)
	{
		if (prefabs.find(prefabName) == prefabs.cend())
		{
			TE_DEBUG_ERROR("There is no \"" + prefabName + "\" prefab.");
		}
		else
		{
			Entity* prefab = (*prefabs[prefabName])(serverSide, params);
			prefab->setEntityTypeName(prefabName);
			return prefab;
		}
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