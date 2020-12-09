#include "PrefabFactory.h"

#include "../../Assets/TestCubePrefab.h"
#include "../../Assets/Voxel/VoxelWorldPrefab.h"
namespace TealEngine 
{
	std::map<std::string, PrefabCreatingFunction> PrefabFactory::prefabs = std::map<std::string, PrefabCreatingFunction>();

	void PrefabFactory::init()
	{
		addPrefab("TestCube", TestCubePrefab::create);
		addPrefab("VoxelWorld", VoxelWorldPrefab::create);
	}
}