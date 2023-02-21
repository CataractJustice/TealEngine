#include "FileSystem/getDirectoryList.h"
#include "FileSystem/getFilesInDir.h"
#include "Resources.h"
#include "System/Debug.h"
#include <map>
#include <fstream>
#include "NlohmannJson/json.hpp"

using Json = nlohmann::json;

namespace TealEngine
{
	namespace Resources
	{
		std::map<std::string, Font> fonts;
		std::map<std::string, Mesh> models;
		std::map<std::string, ShaderProgram> shaders;
		std::map<std::string, Texture> textures;
		std::map<std::string, ShaderProgram> materials;

		std::map<std::string, Font>& getFontsMap() {return fonts;};
		std::map<std::string, Mesh>& getModelsMap() {return models;};
		std::map<std::string, ShaderProgram>& getShadersMap() {return shaders;};
		std::map<std::string, Texture>& getTexturesMap() {return textures;};
		std::map<std::string, ShaderProgram>& getMaterialsMap() {return materials;};

		void load(std::string path)
		{
			TE_DEBUG_INFO("Loading models.");
			//Mesh loading
			vector<string> itemList = getDirectoryList(path + "/models");
			for (int i = 0; i < itemList.size(); i++)
			{
				TE_DEBUG_INFO("Model [" + std::to_string(i) + "/" + std::to_string(itemList.size()) + "]: " + itemList[i]    );
				models[itemList[i]] = Mesh();
				models[itemList[i]].load(path + "/models/" + itemList[i]);
			}

			TE_DEBUG_INFO("Loading shaders.");
			ShaderProgram::loadShadersFromJson(shaders, path + "/shaders/shaderprograms.json");

			TE_DEBUG_INFO("Loading textures.");
			//Textures loading
			itemList = getFilesInDir(path + "/textures", true);
			for (int i = 0; i < itemList.size(); i++)
			{
				textures[itemList[i]] = Texture();
				textures[itemList[i]].loadFromFile(path + "/textures/" +itemList[i]);
				textures[itemList[i]].generateMipmap();
			}

			TE_DEBUG_INFO("Loading fonts.");
			//Fonts loading
			FreeTypeUtil::FreeTypeInitialize();
			itemList = getFilesInDir(path + "/fonts", true);
			for (auto item : itemList) 
			{
				fonts[item] = Font(path + "/fonts/" + item);
				fonts[item].setPixelSizes(64);
			}
		}

		bool isResourceExist(RESOURCE_TYPE resourceType, std::string key) 
		{
			switch (resourceType)
			{
			case TealEngine::Resources::SHADER_RESOURCE:
				return shaders.find(key) != shaders.cend();
				break;
			case TealEngine::Resources::MODEL_RESOURCE:
				return models.find(key) != models.cend();
				break;
			case TealEngine::Resources::TEXTURE_RESOURCE:
				return textures.find(key) != textures.cend();
				break;
			case TealEngine::Resources::MATERIAL_RESOURCE:
				return materials.find(key) != materials.cend();
				break;
			case TealEngine::Resources::FONT_RESOURCE:
				return fonts.find(key) != fonts.cend();
				break;
			}
			return false;
		}

		ShaderProgram& getShader(std::string key) 
		{
			if (!isResourceExist(SHADER_RESOURCE, key)) 
			{
				throw "There is no shader with a \"" + key + "\" as a key.";
			}
			return shaders[key];
		}


		Mesh& getModel(std::string key)
		{
			if (!isResourceExist(MODEL_RESOURCE, key))
			{
				throw "There is no model with a \"" + key + "\" as a key.";
			}
			return models[key];
		}

		Texture& getTexture(std::string key)
		{
			if (!isResourceExist(TEXTURE_RESOURCE, key))
			{
				throw "There is no texture with a \"" + key + "\" as a key.";
			}
			return textures[key];
		}

		void addMaterial(std::string key, const ShaderProgram& material) 
		{
			materials[key] = material;
		}
		
		ShaderProgram& getMaterial(std::string key) 
		{
			if (!isResourceExist(MATERIAL_RESOURCE, key))
			{
				throw "There is no material with a \"" + key + "\" as a key.";
			}
			return materials[key];
		}

		Font& getFont(std::string key) 
		{
			if (!isResourceExist(FONT_RESOURCE, key)) 
			{
				throw "There is no material with a \"" + key + "\" as a key";
			}
			return fonts[key];
		}

		Font& getDefaultFont() 
		{
			return fonts["comici.ttf"];
		}

		const char** getCStrListOfResources(RESOURCE_TYPE type) 
		{
			static const char* list[4096];
			int i = 0;
			switch(type) 
			{
				case RESOURCE_TYPE::FONT_RESOURCE:
					for(auto& p : fonts) 
					{
						list[i++] = p.first.c_str();
					}
				break;

				case RESOURCE_TYPE::MATERIAL_RESOURCE:
					for(auto& p : materials) 
					{
						list[i++] = p.first.c_str();
					}
				break;

				case RESOURCE_TYPE::MODEL_RESOURCE:
					for(auto& p : models) 
					{
						list[i++] = p.first.c_str();
					}
				break;

				case RESOURCE_TYPE::SHADER_RESOURCE:
					for(auto& p : shaders) 
					{
						list[i++] = p.first.c_str();
					}
				break;
				case RESOURCE_TYPE::TEXTURE_RESOURCE:
					for(auto& p : textures) 
					{
						list[i++] = p.first.c_str();
					}
				break;
			}

			return list;
		}

		int getResoucesCount(RESOURCE_TYPE type) 
		{
			switch(type) 
			{
				case RESOURCE_TYPE::FONT_RESOURCE:
					return fonts.size();
				break;

				case RESOURCE_TYPE::MATERIAL_RESOURCE:
					return materials.size();
				break;

				case RESOURCE_TYPE::MODEL_RESOURCE:
					return models.size();
				break;

				case RESOURCE_TYPE::SHADER_RESOURCE:
					return shaders.size();
				break;
				case RESOURCE_TYPE::TEXTURE_RESOURCE:
					return textures.size();
				break;
			}

			return 0;
		}
		
		void renameResouce(RESOURCE_TYPE type, const std::string& nameFrom, const std::string& nameTo) 
		{
			switch(type) 
			{
				case RESOURCE_TYPE::FONT_RESOURCE:
				{
					if(fonts.find(nameFrom) == fonts.cend()) return;
					auto node = fonts.extract(nameFrom);
					node.key() = nameTo;
					fonts.insert(std::move(node));
				}
				break;

				case RESOURCE_TYPE::MATERIAL_RESOURCE:
				{
					if(materials.find(nameFrom) == materials.cend()) return;
					auto node = materials.extract(nameFrom);
					node.key() = nameTo;
					materials.insert(std::move(node));
				}
				break;

				case RESOURCE_TYPE::MODEL_RESOURCE:
				{
					if(models.find(nameFrom) == models.cend()) return;
					auto node = models.extract(nameFrom);
					node.key() = nameTo;
					models.insert(std::move(node));
				}
				break;

				case RESOURCE_TYPE::SHADER_RESOURCE:
				{
					if(shaders.find(nameFrom) == shaders.cend()) return;
					auto node = shaders.extract(nameFrom);
					node.key() = nameTo;
					shaders.insert(std::move(node));
				}
				break;
				case RESOURCE_TYPE::TEXTURE_RESOURCE:
				{
					if(textures.find(nameFrom) == textures.cend()) return;
					auto node = textures.extract(nameFrom);
					node.key() = nameTo;
					textures.insert(std::move(node));
				}
				break;
			}
		}
	}
}