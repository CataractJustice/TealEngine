#include "ObjLoad.h"
#include <fstream>

namespace Kirino
{
	void objLoad(std::string path, 
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	std::vector<unsigned int>& indices
	)
	{
		std::ifstream file = std::ifstream(path);
		std::vector<glm::vec3> readVerts;
		std::vector<glm::vec2> readUvs;
		std::vector<glm::vec3> readNormals;
		std::string word;
		 if (!file)
		{
			throw std::runtime_error("Failed to open obj");
			return;
		}

		while (file >> word) 
		{
			glm::vec3 v3;

			if(word == "v") 
			{
				file >> v3.x;
				file >> v3.y;
				file >> v3.z;
				readVerts.push_back(v3);
			}
			else
			if(word == "vt") 
			{
				file >> v3.x;
				file >> v3.y;
				readUvs.push_back(glm::vec2(v3.x, v3.y));
			}
			else
			if(word == "vn") 
			{
				file >> v3.x;
				file >> v3.y;
				file >> v3.z;
				readNormals.push_back(v3);
			}
			else
			if(word == "f") 
			{
				for (int i = 0; i < 3; i++)
				{
					char triangleStr[256];
					unsigned int readIndices[3];
					file >> triangleStr;
					char* c = triangleStr;
					char* i1 = triangleStr;
					while(*c != '/') c++;
					*c = 0;
					char* i2 = ++c;
					while(*c != '/') c++;
					*c = 0;
					char* i3 = ++c;

					readIndices[0] = (std::atoi(i1) - 1);
					readIndices[1] = (std::atoi(i2) - 1);
					readIndices[2] = (std::atoi(i3) - 1);

					indices.push_back(indices.size());
					vertices.push_back(readVerts[readIndices[0]]);
					uvs.push_back(readUvs[readIndices[1]]);
					normals.push_back(readNormals[readIndices[2]]);
				}
			}
		}
	}
} // namespace Kirino
