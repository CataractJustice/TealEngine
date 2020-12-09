#include "BlockData.h"
#define MAX_NUMBER_OF_BLOCK_TYPES 256
namespace Blocks 
{
	TextureAtlas terrainAtlas;
	BlockData** blocks = new BlockData*[MAX_NUMBER_OF_BLOCK_TYPES]();
	unsigned int blockTypesCount = 0;
	void addBlock(BlockData block) 
	{
		blocks[blockTypesCount] = new BlockData(block);
		blockTypesCount++;
	}

	void init() 
	{
		terrainAtlas.loadAtlas("terrain/Diffuse", vec2(512, 512));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "air"		, EMPTY | TRANSPARRENT));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "grass"	, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "dirt"		, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "stone"	, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "coal"		, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "iron"		, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "silver"	, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "wood"		, 0));
		addBlock(BlockData(vec2(0.0f), vec4(1.0f), 1.0f, "leaves"	, 0));
	}

	unsigned int getBlockID(std::string key) 
	{
		for (unsigned int i = 0; i < blockTypesCount; i++) 
		{
			if (blocks[i]->stringID == key) 
			{
				return i;
			}
		}
	}

	BlockData* getBlockByID(unsigned int id) 
	{
		return blocks[id];
	}
}