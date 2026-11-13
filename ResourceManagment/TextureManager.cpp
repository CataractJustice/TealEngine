#include "TextureManager.h"

namespace TealEngine
{
	void TextureManager::loadImpl(std::filesystem::path path) 
	{
		this->loadedResources[path.filename().string()] = Texture();
		this->loadedResources[path.filename().string()].loadFromFile(path.string());
		this->loadedResources[path.filename().string()].generateMipmap();
	}

	TextureManager::TextureManager() 
	{
		this->extensions.insert({".bmp", ".png", ".jpeg", ".jpg"});
	}
} // namespace TealEngine
