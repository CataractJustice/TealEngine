#pragma once
#include "Filesystem/getDirectoryList.h"
#include "Filesystem/getFilesInDir.h"
#include "Resources.h"
#include "System/Debug.h"
#include <map>
namespace TealEngine
{
	namespace Resources
	{
		std::map<std::string, Font> fonts;
		std::map<std::string, Mesh*> models;
		std::map<std::string, ShaderProgram> shaders;
		std::map<std::string, Texture> textures;
		std::map<std::string, ShaderProgram> materials;

		void MaterialsInit()
		{
		}

		void load(std::string path)
		{
			TE_DEBUG_INFO("Loading models.");
			//Mesh loading
			vector<string> itemList = getDirectoryList(path + "/models");
			for (int i = 0; i < itemList.size(); i++)
			{
				models[itemList[i]] = new Mesh();
				models[itemList[i]]->load(path + "/models/" + itemList[i]);
			}

			TE_DEBUG_INFO("Loading shaders.");
			//Shaders loading
			char shaderName[256], vertName[256], fragName[256];
			FILE* linkListFile = fopen((path + "/shaders/link.txt").c_str(), "r");
			while (fscanf(linkListFile, "%s %s %s", &shaderName, &vertName, &fragName) != EOF)
			{
				Shader vert, frag;
				vert.loadFromFile(path + "/shaders/" + vertName, GL_VERTEX_SHADER);
				frag.loadFromFile(path + "/shaders/" + fragName, GL_FRAGMENT_SHADER);
				shaders[shaderName] = ShaderProgram();
				shaders[shaderName].link(vert.id(), frag.id());
			}
			fclose(linkListFile);

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

			TE_DEBUG_INFO("Generating materials.");
			MaterialsInit();
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


		Mesh* getModel(std::string key)
		{
			if (!isResourceExist(MODEL_RESOURCE, key))
			{
				throw "There is no model with a \"" + key + "\" as a key.";
			}
			return models[key];
		}

		Texture getTexture(std::string key)
		{
			if (!isResourceExist(TEXTURE_RESOURCE, key))
			{
				throw "There is no texture with a \"" + key + "\" as a key.";
			}
			return textures[key];
		}

		void addMaterial(std::string key, ShaderProgram& material) 
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
	}
}