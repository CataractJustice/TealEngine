#include "ShadersManager.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace TealEngine
{
	void ShadersManager::loadImpl(std::filesystem::path path) 
	{
		ShaderProgram::loadShadersFromJson(this->loadedResources, path);
	}

	ShadersManager::ShadersManager() 
	{
		this->extensions.insert({".sp"});
	}
} // namespace TealEngine
