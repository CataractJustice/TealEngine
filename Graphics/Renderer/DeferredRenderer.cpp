#include "DeferredRenderer.h"
#include "../Mesh/MeshUtil.h"
#include "RenderUtil.h"
#include "../Graphics.h"
#include "Graphics/Light/Light.h"
#include "System/Debug.h"
#include "Core.h"
namespace TealEngine {
	void DeferredRenderer::resize(GLuint width, GLuint height)
	{
		ForwardRenderer::resize(width, height);
		if(position.id() == 0) 
		{
			specular = position = normal = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
			light = Texture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT);
		}
		albedo.create(width, height);
		position.create(width, height);
		normal.create(width, height);
		specular.create(width, height);
		light.create(width, height);
		fb.bind();
		fb.attachTexture(albedo.id(), 1);
		fb.attachTexture(position.id(), 2);
		fb.attachTexture(normal.id(), 3);
		fb.attachTexture(specular.id(), 4);
		fb.attachTexture(light.id(), 5);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		fb.unbind();
	}
	void DeferredRenderer::render(GameNode* scene)
	{
		glDepthMask(true);
		if(!activeCamera) return;
		if(this->albedo.getWidth() != activeCamera->renderTexture.getWidth() || this->albedo.getHeight() != activeCamera->renderTexture.getHeight()) 
		{
			this->resize(activeCamera->renderTexture.getWidth(), activeCamera->renderTexture.getHeight());
		}

		fb.bind();
		fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.enable(0);
		fb.enable(1);
		fb.enable(2);
		fb.enable(3);
		fb.enable(4);
		fb.enable(5);
		fb.apply();
		fb.viewport(activeCamera->renderTexture.getWidth(), activeCamera->renderTexture.getHeight());
		applyConfig();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		
		renderModels(scene, nullptr, MeshRenderer::RenderPass::DeferredPass);
		
		//light pass
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		scene->postProcess(getActiveCamera()->renderTexture.id(), albedo.id(), position.id(), normal.id(), specular.id(), light.id(), &(this->fb));
		fb.enable(0);
		fb.apply();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		combineLightShader.setTexture("AlbedoMap", albedo.id());
		combineLightShader.setTexture("LightMap", light.id());
		combineLightShader.setTexture("Dither", Core::textureManager.get("RGBNoise.png").id());
		Render::renderShader(&combineLightShader);

		
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		Render::VP_matrix = activeCamera->getPV();
		
		renderModels(scene, nullptr, MeshRenderer::RenderPass::DebugPass);
		Core::shapesRenderer.renderAll();
		fb.unbind();
	}
}