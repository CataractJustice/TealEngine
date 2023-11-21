#pragma once
#include "GameNode/Component.h"
#include <filesystem>

namespace TealEngine 
{
	class MaterialEditor
	{
		private:
		std::filesystem::path currentMaterialFile;
		std::string currentMaterialName;
		ShaderProgram* currentMaterial;
		int shaderItem;
		public:
		MaterialEditor();
		void render();
	};
}