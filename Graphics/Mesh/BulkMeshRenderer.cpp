#include "BulkMeshRenderer.h"

namespace TealEngine 
{
	BulkMeshRenderer::BulkMeshRenderer() : meshes({})
	{}
	void BulkMeshRenderer::addMesh(Mesh* mesh) 
	{
		this->meshes.emplace(mesh);
	}

	void BulkMeshRenderer::removeMesh(Mesh* mesh) 
	{
		this->meshes.erase(mesh);
	}

	void BulkMeshRenderer::render(ShaderProgram* shader, unsigned int mode, unsigned int LOD) 
	{
		if((!shader && !this->shader)) return;
		ShaderProgram* targetShader = shader ? shader : this->shader;
		targetShader->setUniform("v_mat", Render::V_matrix);
		targetShader->setUniform("pv_mat", Render::VP_matrix);
		targetShader->use();
		for(Mesh* mesh : meshes) 
		{
			if(mesh->getLength() == 0) continue;
			mesh->render(0, mode);
		}
	}
}
