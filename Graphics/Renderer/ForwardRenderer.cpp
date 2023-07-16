#include "ForwardRenderer.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "RenderUtil.h"
#include "Graphics/Graphics.h"
#include "Graphics/Light/Light.h"
#include "System/Input.h"
#include <stack>
namespace TealEngine {

	void ForwardRenderer::resize(GLuint width, GLuint height) 
	{
		this->fb.resize(width, height);
		if(this->depthTexture.id() == 0)this->depthTexture = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
		this->depthTexture.create(width, height);
		fb.attachTexture(0, 0);
		fb.attachDepthTexture(depthTexture.id());
	}

	void ForwardRenderer::render(GameNode* scene, ShaderProgram* material, MeshRenderer::RenderPass pass)
	{
		if(!((unsigned int)pass & (unsigned int)MeshRenderer::RenderPass::ShadowMapPass)) fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.bind();
		applyConfig();
		scene->render(nullptr, (unsigned int)pass);
	}

	void ForwardRenderer::render(list<MeshRenderer*> meshList, ShaderProgram* material, MeshRenderer::RenderPass pass)
	{
		if(!((unsigned int)pass & (unsigned int)MeshRenderer::RenderPass::ShadowMapPass)) fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.bind();
		applyConfig();
		renderModels(meshList, material, pass);
	}

	void ForwardRenderer::renderModels(GameNode* scene, ShaderProgram* shader, MeshRenderer::RenderPass pass)
	{
		scene->render(shader, (unsigned int)pass);
	}

	void ForwardRenderer::renderModels(list<MeshRenderer*> meshList, ShaderProgram* shader, MeshRenderer::RenderPass pass)
	{
		for (MeshRenderer* mesh : meshList)
		{
			if(mesh->isActive() && mesh->isRenderPassActive(pass))
				mesh->render();
		}
	}

	void ForwardRenderer::setCamera(Camera* camera)
	{
		this->activeCamera = camera;
	}

	void ForwardRenderer::setClearColor(glm::vec4 color) 
	{
		this->clearColor = color;
	}

	void ForwardRenderer::setColorClear(bool value)
	{
		this->colorClear = value;
	}

	void ForwardRenderer::setDepthClear(bool value) 
	{
		this->depthClear = value;
	}

	void ForwardRenderer::setStencilClear(bool value)
	{
		this->stencilClear = value;
	}

	void ForwardRenderer::setDepthTest(bool value)
	{
		this->depthTest = value;
	}

	void ForwardRenderer::setStencilTest(bool value)
	{
		this->stencilTest = value;
	}

	Camera* ForwardRenderer::getActiveCamera() 
	{
		return this->activeCamera;
	}

	void ForwardRenderer::applyConfig() 
	{
		glClearDepth(1.0);
		if (this->depthClear) glClear(GL_DEPTH_BUFFER_BIT);
		if (this->stencilClear) glClear(GL_STENCIL_BUFFER_BIT);

		if (this->depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (this->stencilTest) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);

		if (this->colorClear)
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		Render::VP_matrix = activeCamera->getPV();
		Render::V_matrix = activeCamera->getParentOfType<GameNode3D>()->getWorldTransform().getMatrix();
	}
}