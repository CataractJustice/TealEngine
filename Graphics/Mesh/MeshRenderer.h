#pragma once
#include "libs/glm/glm.hpp"
#include "Mesh.h"
#include "Graphics/Renderer/RenderUtil.h"
namespace TealEngine{
	class MeshRenderer : public GameNode
	{
	protected:
		ShaderProgram* shader;
		unsigned int mode;
		

	public:

		MeshRenderer();

		virtual void setShader(ShaderProgram* shader) 
		{
			this->shader = shader;
		}
		void setMode(unsigned int mode) 
		{
			this->mode = mode;
		}

		void static render(SharedMesh* mesh, ShaderProgram* shader, unsigned int mode, unsigned int LOD = 0)
		{
			shader->use();
			mesh->render(LOD, mode);
		}

		virtual void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0)
		{
			render(
				(SharedMesh*)(this->parrent),
				shader ? shader : this->shader,
				mode ? mode : this->mode,
				LOD
			);
		}

	};

	class Mesh3DRenderer : public MeshRenderer 
	{
	private:
		std::map<std::string, Uniform*>::iterator model, nModel, PV;

	public:

		void setShader(ShaderProgram* shader) override
		{
			this->shader = shader;
			this->shader->setUniform("model", mat4(1.0f));
			this->shader->setUniform("n_model", mat4(1.0f));
			this->shader->setUniform("pv_mat", mat4(1.0f));
			model = shader->getUniformIterator("model");
			nModel = shader->getUniformIterator("n_model");
			PV = shader->getUniformIterator("pv_mat");
		}

		void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0) override
		{
			if (((SharedMesh*)(this->parrent))->getLength() > 0) 
			{	
				Transform meshTransform = ((GameNode3D*)this->parrent)->getWorldTransform();
				
				//culling
				vec4 screenPos = Render::VP_matrix * vec4(meshTransform.getPosition(), 1.0f);
				if (true) 
				{
					ShaderProgram* targetShader = shader ? shader : this->shader;
					if (shader)
					{
						targetShader->setUniform("model", meshTransform.getMatrix());
						targetShader->setUniform("n_model", meshTransform.getNormalsModel());
						targetShader->setUniform("pv_mat", Render::VP_matrix);
					}
					else
					{
						targetShader->setUniform(model, meshTransform.getMatrix());
						targetShader->setUniform(nModel, meshTransform.getNormalsModel());
						targetShader->setUniform(PV, Render::VP_matrix);
					}
					MeshRenderer::render(targetShader, mode ? mode : this->mode);
				}
			}
		}
	};
}