#include "MaterialsManager.h"
#include "NlohmannJson/json.hpp"
#include "Core.h"
#include <fstream>
#include "Graphics/MaterialIO.h"
using Json = nlohmann::json;

namespace TealEngine
{
	void MaterialsManager::loadImpl(std::filesystem::path path) 
	{

		Kirino::MaterialIO::MaterialFileInfo fileInfo = Kirino::MaterialIO::info(path);
		//skip invalid
		if(fileInfo.valid == false) return;
		Kirino::Material& material = loadedResources[fileInfo.name];
		std::string name;
		Kirino::MaterialIO::load(material, name, path);
		/*
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
		Kirino::Material& material = this->loadedResources[name];
		material.setProgram(&Core::shadersManager.get(materialJson["shader"]));
		const auto& uniforms = material.getShaderProgram()->getUniforms();
		for(const auto& uniform : materialJson["uniforms"]) 
		{
			const std::string& uName = uniform["name"];
			auto uIt = std::find_if(uniforms.begin(), uniforms.end(), [&uName](const Kirino::ShaderProgram::UniformRecord& item){
				return item.name == uName;
			});
			if(uIt == uniforms.cend())
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
			
			if(uIt->type != loadedUniformTypeEnum) continue;
			
			float uf1 = 0.0;
			int ui1 = 0;
			glm::vec2 uf2(0.0, 0.0);
			glm::vec3 uf3(0.0, 0.0, 0.0);
			glm::vec4 uf4(0.0, 0.0, 0.0, 0.0);

			switch (loadedUniformTypeEnum)
			{
			case GL_FLOAT:
				uf1 = uniform["value"].get<float>();
				material.setUniform(uniform["name"].get<std::string>(), &uf1, sizeof(uf1), 0);
				break;
			case GL_INT:
				ui1 = uniform["value"].get<int>();
				material.setUniform(uniform["name"].get<std::string>(), &ui1, sizeof(ui1), 0);
				break;
			case GL_FLOAT_VEC2:
				uf2 = glm::vec2(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>()
				);
				material.setUniform(uniform["name"].get<std::string>(), &uf2, sizeof(uf2), 0);
				break;
			case GL_FLOAT_VEC3:
				uf3 = glm::vec3(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>(),
					uniform["value"][2].get<float>()
				);
				material.setUniform(uniform["name"].get<std::string>(), &uf3, sizeof(uf3), 0);
				break;
			case GL_FLOAT_VEC4:
				uf4 = glm::vec4(
					uniform["value"][0].get<float>(),
					uniform["value"][1].get<float>(),
					uniform["value"][2].get<float>(),
					uniform["value"][3].get<float>()
				);
				material.setUniform(uniform["name"].get<std::string>(), &uf4, sizeof(uf4), 0);
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
			std::string n = nIt->get<std::string>();
			std::string t = tIt->get<std::string>();
			const std::string& tName = texture["name"];
			const auto& samplers = material.getShaderProgram()->getSamplers();
			auto tMatIt = std::find_if(samplers.begin(), samplers.end(), [tName](const Kirino::ShaderProgram::UniformRecord& item){
				return item.name == tName;
			});
			if(tMatIt == uniforms.cend())
				continue;
			unsigned int textureId = Core::textureManager.get(texture["texture"].get<std::string>()).id();
			material.setTexture(texture["name"], &textureId, 1, 0);
		}*/
		for(auto ubo : defaultUBOs) 
		{
			material.setUniformBlock(ubo.first, ubo.second);
		}
	}

	MaterialsManager::MaterialsManager() 
	{
		this->extensions.insert({".mbf"});
	}
} // namespace TealEngine
