#pragma once
#include <string>
#include <vector>
#include "libs/glm/glm.hpp"
namespace Kirino
{
	void objLoad(std::string path, 
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	std::vector<unsigned int>& indices
	);
} // namespace Kirino
