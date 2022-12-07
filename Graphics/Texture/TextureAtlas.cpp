#include "TextureAtlas.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
namespace TealEngine
{
	FrameBuffer fb;

	void TextureAtlas::loadAtlas(std::string path, vec2 resolution)
	{
		std::vector<std::string> tlist = getDirectoryList(path);
		textures = new Texture[tlist.size()];

		vec2 s = vec2(resolution.x * float(tlist.size()), resolution.y);
		this->create(s.x, s.y);
		fb.resize(s.x, s.y);
		fb.attachTexture(this->id(), 0);
		fb.enable(0);
		fb.apply();

		for (unsigned int i = 0; i < tlist.size(); i++)
		{
			textures[i] = Resources::getTexture(path + "/" + tlist[i]);
			Render::renderTexture(textures[i].id(), vec2(float(i) / float(tlist.size()), 0.0f), vec2(1.0f / float(tlist.size()), 1.0f));
		}
	}
}