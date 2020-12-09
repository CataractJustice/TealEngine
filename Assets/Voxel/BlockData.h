#pragma once
#include "../../libs/glm/vec2.hpp"
#include "../../libs/glm/vec3.hpp"
#include "../../libs/glm/vec4.hpp"
#include <string>
#include "../../Graphics/Texture/TextureAtlas.h"
using namespace glm;
using namespace TealEngine;
namespace Blocks 
{
	enum BlockPropertysBitmap
	{
		EMPTY = 1,
		TRANSPARRENT = 1 << 1
	};

	struct BlockData
	{
	public:
		BlockData(vec2 uv, vec4 color, float hardness, std::string stringID, int propertysBitmap) : 
			uv(uv), 
			color(color), 
			hardness(hardness), 
			stringID(stringID),
			propertysBitmap(propertysBitmap)
		{
		
		}
		vec2 uv;
		vec4 color;
		float hardness;
		std::string stringID;
		int propertysBitmap;
	};

	void init();
	unsigned int getBlockID(std::string key);
	BlockData* getBlockByID(unsigned int);
}