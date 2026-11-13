#pragma once
#include "ResourceManager.h"
#include "Kirino/ShaderProgram.h"
#include "Kirino/Shader.h"

namespace TealEngine
{
	class ShadersManager : public ResourceManager<Kirino::ShaderProgram> 
	{
		private:
		std::map<std::string, Kirino::Shader> shaders;
		void loadImpl(std::filesystem::path path) override;
		public: 
		ShadersManager();
	};
} // namespace TealEngine
