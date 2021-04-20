#pragma once
#include <string>
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/GUI/Text/Font.h"
namespace TealEngine 
{
	namespace Resources 
	{
		enum RESOURCE_TYPE
		{
			SHADER_RESOURCE,
			MODEL_RESOURCE,
			TEXTURE_RESOURCE,
			MATERIAL_RESOURCE,
			FONT_RESOURCE
		};

		void load(std::string path);
		ShaderProgram& getShader(std::string key);
		Mesh* getModel(std::string key);
		Texture getTexture(std::string key);
		ShaderProgram& getMaterial(std::string key);
		Font getFont(std::string key);
		void addMaterial(std::string key, ShaderProgram material);
		bool isResourceExist(RESOURCE_TYPE resourceType,  std::string key);
	}
}