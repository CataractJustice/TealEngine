#pragma once
#include "GameNode/Component.h"
#include "Kirino/Material.h"
#include <filesystem>

namespace TealEngine 
{
	class MaterialEditor
	{
		private:
		std::filesystem::path currentMaterialFile;
		std::string currentMaterialName;
		Kirino::Material* currentMaterial;
		int shaderItem;
		public:
		MaterialEditor();
		void render();
	};
}