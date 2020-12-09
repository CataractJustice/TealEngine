#pragma once
#include <string>
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Texture/Texture.h"
namespace TealEngine 
{
	namespace Resources 
	{
		enum RESOURCE_TYPE
		{
			SHADER_RESOURCE,
			MODEL_RESOURCE,
			TEXTURE_RESOURCE
		};

		void load(std::string path);
		ShaderProgram getShader(std::string key);
		Mesh* getModel(std::string key);
		Texture getTexture(std::string key);
		void addMaterial(std::string key, ShaderProgram material);
		ShaderProgram getMaterial(std::string key);
		bool isResourceExist(RESOURCE_TYPE resourceType,  std::string key);
	}
}