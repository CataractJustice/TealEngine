#pragma once
#include "libs/glm/glm.hpp"
#include "Mesh.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "MeshRenderer.h"
namespace TealEngine{
	class MeshRenderer : public Component
	{
	private:

	protected:
		SharedMesh* mesh;
		ShaderProgram* shader;
		unsigned int mode;

		unsigned int activeRenderStages;

	public:
		RegisterComponent();
		MeshRenderer();

		enum class RenderStage
		{
			Lit = 1,
			Unlit = 1 << 1
		};

		void setRenderStage(RenderStage stage, bool value) 
		{
			activeRenderStages = value ? (activeRenderStages | (unsigned int)stage) : (activeRenderStages & (unsigned int)stage);
		}

		bool getRenderStage(RenderStage stage) 
		{
			return bool(activeRenderStages & (unsigned int)stage) || !(int)stage;
		}

		virtual void setShader(ShaderProgram* shader) 
		{
			this->shader = shader;
		}

		ShaderProgram* getShader() 
		{
			return this->shader;
		}

		void setMode(unsigned int mode) 
		{
			this->mode = mode;
		}

		void setMesh(SharedMesh* mesh) {
			this->mesh = mesh;
		}

		void static render(SharedMesh* mesh, ShaderProgram* shader, unsigned int mode, unsigned int LOD = 0)
		{
			shader->use();
			mesh->render(LOD, mode);
		}

		virtual void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0)
		{
			if((this->shader || shader) && this->mesh)
				render(
					mesh,
					shader ? shader : this->shader,
					mode ? mode : this->mode,
					LOD
				);
		}

		virtual void render(ShaderProgram* shader, unsigned int stages) override 
		{
			this->render(shader, this->mode, 0);
		}
	};

	class Mesh3DRenderer : public MeshRenderer 
	{
	private:
		std::map<std::string, Uniform>::iterator model, nModel, PV, V;

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

		void onPropSet(const std::string& propName) override
		{
			if(shader)
				this->setShader(this->shader);
		}
		void renderId() override;
		
		void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0) override
		{
			if((!shader && !this->shader) || !this->mesh) return;
			if (mesh->getLength() > 0) 
			{	
				Transform meshTransform = ((GameNode3D*)getParent())->getWorldTransform();
				
				vec4 screenPos = Render::VP_matrix * vec4(meshTransform.getPosition(), 1.0f);
				//to-do: culling
				if (true) 
				{
					ShaderProgram* targetShader = shader ? shader : this->shader;
					
					//to-do safely cache uniform iterators
					if (shader || true)
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
					MeshRenderer::render(targetShader, mode ? mode : this->mode, 0);
				}
			}
		}
	};
}