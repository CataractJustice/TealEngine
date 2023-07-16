#pragma once
#include "GameNode/Component.h"
#include <filesystem>

namespace TealEngine 
{
	class MaterialEditor : public Component
	{
		private:
		std::filesystem::path currentMaterialFile;
		std::string currentMaterialName;
		ShaderProgram* currentMaterial;
		int shaderItem;
		public:
		MaterialEditor();
		void imGuiRender(const std::string& windowName) override;
	};
}