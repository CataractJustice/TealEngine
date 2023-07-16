#include "ModelsManager.h"

namespace TealEngine
{
	void ModelsManager::loadImpl(std::filesystem::path path) 
	{
		Mesh& mesh = this->meshSources[path];
		mesh.load(path);
		mesh.calcNormals();
		mesh.calcTangents();
		this->loadedResources[path.filename()] = SharedMesh(*mesh.sharedMesh());
	}

	ModelsManager::ModelsManager() 
	{
		this->extensions.insert({".obj"});
	}

	void ModelsManager::clearAll() 
	{
		this->loadedResources.clear();
		this->meshSources.clear();
	}

} // namespace TealEngine
