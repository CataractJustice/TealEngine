#pragma once
#include "Filesystem/getDirectoryList.h"
#include "Filesystem/getFilesInDir.h"
#include "Resources.h"
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
			ShaderProgram testShader = Resources::getShader("deffered");
			testShader.setTexture("tex", Resources::getTexture("stone.png").id());
			materials["testShader"] = testShader;

			ShaderProgram sandShader = Resources::getShader("deffered");
			sandShader.setTexture("tex", Resources::getTexture("sand.bmp").id());
			sandShader.setTexture("nmap", Resources::getTexture("tileNormalMap.jpg").id());
			sandShader.setTexture("specmap", Resources::getTexture("bd.bmp").id());
			sandShader.setUniform("spec", vec3(0.3f, 256.0f, 1.0f));
			sandShader.setUniform("color", vec4(1.0f));
			materials["sandShader"] = sandShader;

			ShaderProgram playerShader = Resources::getShader("deffered");
			playerShader.setTexture("tex", Resources::getTexture("obama.bmp").id());
			playerShader.setTexture("nmap", Resources::getTexture("moonnorm.jpg").id());
			materials["SokobanPlayer"] = playerShader;

			ShaderProgram boxShader = Resources::getShader("deffered");
			boxShader.setTexture("tex", Resources::getTexture("boxDiffuseMap.jpg").id());
			boxShader.setTexture("nmap", Resources::getTexture("boxNormalMap.jpg").id());
			boxShader.setUniform("color", vec4(1.0f));
			materials["SokobanBox"] = boxShader;

			ShaderProgram tarShader = Resources::getShader("deffered");
			tarShader.setTexture("tex", Resources::getTexture("bd.bmp").id());
			tarShader.setTexture("nmap", Resources::getTexture("bn.bmp").id());
			tarShader.setUniform("color", vec4(0.0f, 1.0f, 0.0f, 0.0f));
			materials["SokobanTarget"] = tarShader;
		}

		void load(std::string path)
		{
			//Mesh loading
			vector<string> itemList = getDirectoryList(path + "/models");
			for (int i = 0; i < itemList.size(); i++)
			{
				models[itemList[i]] = new Mesh();
				models[itemList[i]]->load(path + "/models/" + itemList[i]);
			}

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

			//Textures loading
			itemList = getFilesInDir(path + "/textures", true);
			for (int i = 0; i < itemList.size(); i++)
			{
				textures[itemList[i]] = Texture();
				textures[itemList[i]].loadFromFile(path + "/textures/" +itemList[i]);
				textures[itemList[i]].generateMipmap();
			}

			//Fonts loading
			FreeTypeUtil::FreeTypeInitialize();
			itemList = getFilesInDir(path + "/fonts", true);
			for (auto item : itemList) 
			{
				fonts[item] = Font(path + "/fonts/" + item);
				fonts[item].setPixelSizes(64);
			}

			MaterialsInit();
		}

		bool isResourceExist(RESOURCE_TYPE resourceType, std::string key) 
		{
			switch (resourceType)
			{
			case TealEngine::Resources::SHADER_RESOURCE:
				return shaders.find(key) != shaders.end();
				break;

			case TealEngine::Resources::MODEL_RESOURCE:
				return models.find(key) != models.end();
				break;

			case TealEngine::Resources::TEXTURE_RESOURCE:
				return textures.find(key) != textures.end();
				break;
			case TealEngine::Resources::MATERIAL_RESOURCE:
				return materials.find(key) != materials.end();
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

		void addMaterial(std::string key, ShaderProgram material) 
		{
			materials[key];
		}
		
		ShaderProgram& getMaterial(std::string key) 
		{
			return materials[key];
		}

		Font getFont(std::string key) 
		{
			return fonts[key];
		}
	}
}