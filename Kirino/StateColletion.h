#pragma once
#include "TextureBindings.h"
#include "UBOBindings.h"
#include "Material.h"

namespace Kirino 
{
	struct StateColletion 
	{
		unsigned int vao;
		const ShaderProgram* shader;
		Material* usedMaterial;
		Material* forcedMaterial;
		TextureBindings textureBindings;
		UBOBindings uboBindings;
	};
};