#pragma once
#include "ResourceManager.h"
#include "Kirino/Mesh.h"
#include "Kirino/MeshBuilders/Mesh3DBuilder.h"
#include <map>
namespace TealEngine
{
	class ModelsManager : public ResourceManager<Kirino::Mesh> 
	{
		private:
		void loadImpl(std::filesystem::path path) override;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		Kirino::Mesh3DBuilder meshSource;
		std::map<std::string, std::pair<unsigned int, unsigned int>> meshBounds;
		public: 
		ModelsManager();
		void clearAll() override;
		void build();
	};
} // namespace TealEngine
