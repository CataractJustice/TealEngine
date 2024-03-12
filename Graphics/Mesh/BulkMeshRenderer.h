#pragma once
#include "MeshRenderer.h"
#include <vector>
namespace TealEngine 
{
	class BulkMeshRenderer : public MeshRenderer 
	{
	private:
		std::set<Mesh*> meshes;
	public:
		BulkMeshRenderer();
		void addMesh(Mesh* mesh);
		void removeMesh(Mesh* mesh);
		
		void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0) override;
	};
}