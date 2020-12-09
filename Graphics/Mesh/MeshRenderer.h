#pragma once
#include "../../libs/glm/glm.hpp"
#include "Mesh.h"
#include "../Renderer/RenderUtil.h"
namespace TealEngine{
	class MeshRenderer : public GameNode
	{
	protected:
		ShaderProgram* shader;
		GLenum mode;
		

	public:

		MeshRenderer() 
		{
			this->addTag("MeshRenderer");
			this->mode = GL_TRIANGLES;
		}

		virtual void setShader(ShaderProgram* shader) 
		{
			this->shader = shader;
		}
		void setMode(GLenum mode) 
		{
			this->mode = mode;
		}

		void static render(SharedMesh* mesh, ShaderProgram* shader, GLenum mode, GLuint LOD = 0)
		{
			shader->use();
			mesh->render(LOD, mode);
		}

		virtual void render(ShaderProgram* shader = nullptr, GLenum mode = 0, GLuint LOD = 0)
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
		std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator model, nModel, PV;

	public:

		void setShader(ShaderProgram* shader) override
		{
			this->shader = shader;
			this->shader->setUniform<glm::mat4>("model", &mat4(1.0f));
			this->shader->setUniform<glm::mat4>("n_model", &mat4(1.0f));
			this->shader->setUniform<glm::mat4>("pv_mat", &mat4(1.0f));
			model = shader->getUniformIterator("model");
			nModel = shader->getUniformIterator("n_model");
			PV = shader->getUniformIterator("pv_mat");
		}

		void render(ShaderProgram* shader = nullptr, GLenum mode = 0, GLuint LOD = 0) override
		{
			if (((SharedMesh*)(this->parrent))->getLength() > 0) 
			{	
				Transform meshTransform = ((GameNode3D*)this->parrent)->getWorldTransform();
				
				//culling
				vec4 screenPos = Render::VP_matrix * vec4(meshTransform.getPosition(), 1.0f);
				if (screenPos.z > -32.0f) 
				{
					ShaderProgram* targetShader = shader ? shader : this->shader;
					if (shader)
					{
						targetShader->setUniform<glm::mat4>("model", &meshTransform.getMatrix());
						targetShader->setUniform<glm::mat4>("n_model", &meshTransform.getNormalsModel());
						targetShader->setUniform<glm::mat4>("pv_mat", &Render::VP_matrix);
					}
					else
					{
						targetShader->setUniform<glm::mat4>(model, &meshTransform.getMatrix());
						targetShader->setUniform<glm::mat4>(nModel, &meshTransform.getNormalsModel());
						targetShader->setUniform<glm::mat4>(PV, &Render::VP_matrix);
					}
					MeshRenderer::render(targetShader, mode ? mode : this->mode, clamp(int(log2f(length(vec3(screenPos.x, screenPos.y, screenPos.z)) / 16.0f + 2.0f)) - 2, 0, int(((SharedMesh*)parrent)->getLODCount() - 1)));
				}
			}
		}
	};
}