#include "ShadersManager.h"
#include "NlohmannJson/json.hpp"
#include "FileSystem/loadStrFromFile.h"
#include <fstream>
#include <GL/glew.h>
using Json = nlohmann::json;

namespace TealEngine
{
	void ShadersManager::loadImpl(std::filesystem::path path) 
	{
		std::ifstream shadersFile(path);
		Json json = Json::parse(shadersFile);
		for(auto& shaderJson : json.items()) 
		{
			if(loadedResources.find(shaderJson.key()) != loadedResources.cend()) continue;

			const auto vertPath = path.parent_path() / shaderJson.value()["vert"].get<std::string>();
			const auto fragPath = path.parent_path() / shaderJson.value()["frag"].get<std::string>();
			Kirino::Shader& vert = shaders[vertPath.string()];
			Kirino::Shader& frag = shaders[fragPath.string()];
			if(vert.getId() == 0) 
			{
				vert.compile(loadStrFromFile(vertPath).c_str(), GL_VERTEX_SHADER);
			}
			if(frag.getId() == 0) 
			{
				frag.compile(loadStrFromFile(fragPath).c_str(), GL_FRAGMENT_SHADER);
			}
			Kirino::ShaderProgram& program = loadedResources[shaderJson.key()];
			program.link(vert.getId(), frag.getId());
		}
	}

	ShadersManager::ShadersManager() 
	{
		this->extensions.insert({".sp"});
	}
} // namespace TealEngine
