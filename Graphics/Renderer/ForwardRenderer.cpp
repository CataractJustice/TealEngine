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

	void ForwardRenderer::render(list<MeshRenderer*> meshList, ShaderProgram* material, bool depthOnly)
	{
		if(!depthOnly)fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.bind();
		applyConfig();
		renderModels(meshList, material);
	}

	void ForwardRenderer::render(ShaderProgram* material)
	{
		renderModels(material);
	}

	void ForwardRenderer::pop(GameNode* scene)
	{
		vector<GameNode*> nodes = scene->getAllChilds();
		nodes.push_back(scene);
		for (auto node : nodes)
		{
			std::vector<MeshRenderer*> meshes = node->findComponentsByType<MeshRenderer>();
			for (auto mesh : meshes) 
			{
				meshList.remove(mesh);
			}
		}
	}

	void ForwardRenderer::push(GameNode* scene)
	{
		vector<GameNode*> nodes = scene->getAllChilds();
		nodes.push_back(scene);
		for (auto node : nodes)
		{
			std::vector<MeshRenderer*> meshes = node->findComponentsByType<MeshRenderer>();
			for (auto mesh : meshes)
			{
				meshList.push_back(mesh);
			}
		}
	}

	void ForwardRenderer::renderModels(ShaderProgram* shader)
	{
		for (MeshRenderer* mesh : meshList)
		{
			mesh->render(nullptr, Input::Keyboard::isKeyPressed(GLFW_KEY_9) ? GL_LINES : GL_TRIANGLES);
		}
	}

	void ForwardRenderer::renderModels(list<MeshRenderer*> meshList, ShaderProgram* shader)
	{
		for (MeshRenderer* mesh : meshList)
		{
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
	}

	std::list<MeshRenderer*>& ForwardRenderer::getMeshRendererList() 
	{
		return this->meshList;
	}
}