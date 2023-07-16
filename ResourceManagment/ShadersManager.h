#pragma once
#include "ResourceManager.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace TealEngine
{
	class ShadersManager : public ResourceManager<ShaderProgram> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;
		public: 
		ShadersManager();
	};
} // namespace TealEngine
