#pragma once
#include "ResourceManager.h"
#include "Graphics/Mesh/Mesh.h"

namespace TealEngine
{
	class ModelsManager : public ResourceManager<SharedMesh> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;
		std::map<std::string, Mesh> meshSources;
		public: 
		ModelsManager();
		void clearAll() override;
	};
} // namespace TealEngine
