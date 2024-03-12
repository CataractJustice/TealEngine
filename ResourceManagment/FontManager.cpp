#include "FontManager.h"

namespace TealEngine
{
	void FontManager::loadImpl(std::filesystem::path path) 
	{
		this->loadedResources[path.filename()] = Font(path);
	}

	FontManager::FontManager() 
	{
		this->extensions.insert({".ttf", ".otf"});
	}
} // namespace TealEngine
