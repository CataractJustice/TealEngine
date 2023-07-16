#pragma once
#include "ResourceManager.h"
#include "Graphics/Texture/Texture.h"

namespace TealEngine
{
	class TextureManager : public ResourceManager<Texture> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;

		public: 
		TextureManager();
	};
} // namespace TealEngine
