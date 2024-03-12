#include "MaterialsManager.h"
#include "NlohmannJson/json.hpp"
#include "Core.h"
#include <fstream>
using Json = nlohmann::json;

namespace TealEngine
{
	void MaterialsManager::loadImpl(std::filesystem::path path) 
	{
		if(!std::filesystem::is_regular_file(path)) return;
		Json materialJson;
		std::ifstream file(path);
		try 
		{
			materialJson = Json::parse(file);
		}
		catch(...)
		{
			return;
		}
		if(materialJson.find("name") == materialJson.cend()) return;
		std::string name = materialJson["name"];
		ShaderProgram& material = this->loadedResources[name] = Core::shadersManager.get(materialJson["shader"]);

		for(const auto& uniform : materialJson["uniforms"]) 
		{
			//uniform iterator
			auto uIt = material.getUniformsMap().find(uniform["name"]);
			if(uIt == material.getUniformsMap().cend())
				continue;

			std::string loadedUniformType = uniform["type"];
			int loadedUniformTypeEnum = -1;
			if(loadedUniformType == "float") 
			{
				loadedUniformTypeEnum = GL_FLOAT;
			}
			else if(loadedUniformType == "int")
			{
				loadedUniformTypeEnum = GL_INT;
			}
			else if(loadedUniformType == "vec2")
			{
				loadedUniformTypeEnum = GL_FLOAT_VEC2;
			}
			else if(loadedUniformType == "vec3")
			{
				loadedUniformTypeEnum = GL_FLOAT_VEC3;
			}
			else if(loadedUniformType == "vec4")
			{
				loadedUniformTypeEnum = GL_FLOAT_VEC4;
			}
			
			if(uIt->second.getType() != loadedUniformTypeEnum) continue;
			
			float uf1 = 0.0;
			int ui1 = 0;
			glm::vec2 uf2(0.0, 0.0);
			glm::vec3 uf3(0.0, 0.0, 0.0);
			glm::vec4 uf4(0.0, 0.0, 0.0, 0.0);

			switch (loadedUniformTypeEnum)
			{
			case GL_FLOAT:
				uf1 = uniform["value"].get<float>();
				material.setUniform(uniform["name"], uf1);
				break;
			case GL_INT:
				ui1 = uniform["value"].get<int>();
				material.setUniform(uniform["name"], ui1);
				break;
			case GL_FLOAT_VEC2:
				uf2 = vec2(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>()
				);
				material.setUniform(uniform["name"], uf2);
				break;
			case GL_FLOAT_VEC3:
				uf3 = vec3(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>(),
					uniform["value"][2].get<float>()
				);
				material.setUniform(uniform["name"], uf3);
				break;
			case GL_FLOAT_VEC4:
				uf4 = vec4(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>(),
					uniform["value"][2].get<float>(),
					uniform["value"][3].get<float>()
				);
				material.setUniform(uniform["name"], uf4);
				break;
			default:
				break;
			}
		}

		for(const auto& texture : materialJson["textures"]) 
		{
			auto nIt = texture.find("name");
			auto tIt = texture.find("texture");
			if(nIt == texture.cend() || tIt == texture.cend()) continue;
			std::string n = *nIt;
			std::string t = *tIt;
			auto& tm = material.getTextureMap();
			if(material.getTextureMap().find(texture["name"]) == tm.cend()) 
			{
				continue;
			}

			material.setTexture(texture["name"], Core::textureManager.get(texture["texture"]).id());
		}
	}

	MaterialsManager::MaterialsManager() 
	{
		this->extensions.insert({".mtljson"});
	}
} // namespace TealEngine
