#pragma once
#include "GameNode/Component.h"
#include <filesystem>
#include "Graphics/Texture/Texture.h"
namespace TealEngine 
{
	class GameAssetsBrowser
	{
		private:
		std::filesystem::path currentPath;
		std::filesystem::path rootPath;
		static bool isResourcesLoaded;
		static Texture folderIcon, textIcon, cppIcon, imageIcon, nodesIcon, unknownIcon;
		std::string renameTarget;
		std::string renameInput;
		bool isFocusedRenameInputOnce;
		public:
		void setPath(std::filesystem::path path);
		void setRootPath(std::filesystem::path path);
		void render();
		GameAssetsBrowser();
	};
}