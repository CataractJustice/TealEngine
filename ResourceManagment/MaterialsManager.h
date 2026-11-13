#pragma once
#include "ResourceManager.h"
#include "Kirino/Material.h"
#include <map>
#include <string>

namespace TealEngine
{
	class MaterialsManager : public ResourceManager<Kirino::Material> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;
		std::map<std::string, unsigned int> defaultUBOs;
		public: 
		MaterialsManager();
		inline void addDefaultUBO(const std::string& uboName, unsigned int uboId) 
		{
			defaultUBOs[uboName] = uboId;
		};
		inline const std::map<std::string, unsigned int>& getDefaultUBOs() const { return defaultUBOs; }
	};
} // namespace TealEngine
