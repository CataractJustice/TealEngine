#pragma once
#include "Texture.h"
#include "FileSystem/getDirectoryList.h"
#include "Resources.h"
#include "Graphics/Renderer/RenderUtil.h"

namespace TealEngine 
{
	class TextureAtlas : public Texture 
	{
	private:
		Texture* textures;
		std::map<std::string, vec2> offsets;
		float size;

	public:
		void loadAtlas(std::string path, vec2 resolution);
	};
}