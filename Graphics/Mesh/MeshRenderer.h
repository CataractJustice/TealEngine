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

		unsigned int renderPassMask;
		bool shadowCaster;

	public:
		enum class RenderPass 
		{
			DeferredPass = 1,
			ShadowMapPass = 2,
			UnlitPass = 4,
			DebugPass = 8
		};
		MeshRenderer();

		void setRenderPassActive(MeshRenderer::RenderPass pass, bool value) 
		{
			renderPassMask = value ? (renderPassMask | (unsigned int)pass) : (renderPassMask & ~((unsigned int)pass));
		}

		bool isRenderPassActive(MeshRenderer::RenderPass pass) 
		{
			return bool(renderPassMask & (unsigned int)pass) || !(int)pass;
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
			if(mesh->getLength() == 0) return;
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

		virtual void render(ShaderProgram* shader, unsigned int passes) override 
		{
			if(this->renderPassMask & passes)
				this->render(shader, this->mode, 0);
		}

		void onPropSet(const std::string& propName) override
		{
			if(shader)
				this->setShader(this->shader);
			this->setRenderPassActive(RenderPass::ShadowMapPass, this->isShadowCaster());
		}

		bool isShadowCaster();
		void setShadowCast(bool castShadow);


	};

	class Mesh3DRenderer : public MeshRenderer 
	{
	private:
		std::map<std::string, Uniform>::iterator model, nModel, PV, V;

	public:

		void setShader(ShaderProgram* shader) override
		{
			this->shader = shader;
			if(this->shader == nullptr) return;
			this->shader->setUniform("model", mat4(1.0f));
			this->shader->setUniform("n_model", mat4(1.0f));
			this->shader->setUniform("pv_mat", mat4(1.0f));
			model = shader->getUniformIterator("model");
			nModel = shader->getUniformIterator("n_model");
			PV = shader->getUniformIterator("pv_mat");
		}

		void renderId() override;
		
		void render(ShaderProgram* shader = nullptr, unsigned int mode = 0, unsigned int LOD = 0) override
		{
			if((!shader && !this->shader) || !this->mesh) return;
			if (mesh->getLength() > 0) 
			{	
				Transform meshTransform = ((GameNode3D*)getParent())->getWorldTransform();
				
				vec4 screenPos = Render::VP_matrix * vec4(meshTransform.getPosition(), 1.0f);
				
				ShaderProgram* targetShader = shader ? shader : this->shader;
				
				targetShader->setUniform("model", meshTransform.getMatrix());
				targetShader->setUniform("n_model", meshTransform.getNormalsModel());
				targetShader->setUniform("pv_mat", Render::VP_matrix);
				MeshRenderer::render(targetShader, mode ? mode : this->mode, 0);
			}
		}
	};
}