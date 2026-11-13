#include "ModelsManager.h"
#include "Kirino/ObjLoad.h"
namespace TealEngine
{
	void ModelsManager::loadImpl(std::filesystem::path path) 
	{
		unsigned int meshStart = indices.size();
		Kirino::objLoad(path.string(), vertices, uvs, normals, indices);
		unsigned int meshEnd = indices.size();
		this->meshBounds[path.filename().string()] = {meshStart, meshEnd};
	}

	ModelsManager::ModelsManager() 
	{
		this->extensions.insert({".obj"});
	}

	void ModelsManager::clearAll() 
	{
		this->loadedResources.clear();
		vertices.clear();
		uvs.clear();
		normals.clear();
		indices.clear();
	}

	void ModelsManager::build() 
	{
		meshSource.buildFromVectors(vertices, uvs, normals, indices);
		for(auto kval : this->meshBounds) 
		{
			Kirino::Mesh& mesh = loadedResources[kval.first];
			mesh.indexCount = kval.second.second - kval.second.first;
			mesh.indexOffset = 0;//kval.second.first;
			mesh.vertexOffset = kval.second.first;
			mesh.vao = meshSource.getMesh().vao;
			for(int i = kval.second.first; i < kval.second.second; i++) 
			{
				mesh.boundingBox.maxX = std::max(mesh.boundingBox.maxX, vertices[i].x);
				mesh.boundingBox.maxY = std::max(mesh.boundingBox.maxY, vertices[i].y);
				mesh.boundingBox.maxZ = std::max(mesh.boundingBox.maxZ, vertices[i].z);
				mesh.boundingBox.minX = std::min(mesh.boundingBox.minX, vertices[i].x);
				mesh.boundingBox.minY = std::min(mesh.boundingBox.minY, vertices[i].y);
				mesh.boundingBox.minZ = std::min(mesh.boundingBox.minZ, vertices[i].z);
			}
		}
	}

} // namespace TealEngine
