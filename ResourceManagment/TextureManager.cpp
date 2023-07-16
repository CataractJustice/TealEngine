#include "TextureManager.h"

namespace TealEngine
{
	void TextureManager::loadImpl(std::filesystem::path path) 
	{
		this->loadedResources[path.filename()] = Texture();
		this->loadedResources[path.filename()].loadFromFile(path.string());
		this->loadedResources[path.filename()].generateMipmap();
	}

	TextureManager::TextureManager() 
	{
		this->extensions.insert({".bmp", ".png", ".jpeg", ".jpg"});
	}
} // namespace TealEngine
