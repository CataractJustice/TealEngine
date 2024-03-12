#pragma once
#include "ResourceManager.h"
#include "Graphics/GUI/Text/Font.h"

namespace TealEngine
{
	class FontManager : public ResourceManager<Font> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;
		public: 
		FontManager();
	};
} // namespace TealEngine
