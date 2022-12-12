#include "DefferedRenderer.h"
#include "../Mesh/MeshUtil.h"
#include "RenderUtil.h"
#include "../Graphics.h"
#include "Graphics/Light/Light.h"
#include "Resources.h"
#include "System/Debug.h"
namespace TealEngine {
	void DefferedRenderer::resize(GLuint width, GLuint height)
	{
		ForwardRenderer::resize(width, height);
		if(position.id() == 0)specular = position = normal = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
		albedo.create(width, height);
		position.create(width, height);
		normal.create(width, height);
		specular.create(width, height);
		light.create(width, height);
		fb.attachTexture(albedo.id(), 1);
		fb.attachTexture(position.id(), 2);
		fb.attachTexture(normal.id(), 3);
		fb.attachTexture(specular.id(), 4);
		fb.attachTexture(light.id(), 5);
		fb.apply();
		shadowMapRenderer.resize(width, height);
		shadowMapRenderer.setDepthClear(true);
		shadowMapRenderer.setDepthTest(true);
		shadowMapRenderer.fb.bind();
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	void DefferedRenderer::render(GameNode* scene)
	{
		if(!activeCamera) return;
		//TE_DEBUG_INFO("Configuring framebuffer.");
		fb.bind();
		fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.enable(0);
		fb.enable(1);
		fb.enable(2);
		fb.enable(3);
		fb.enable(4);
		fb.enable(5);
		fb.apply();
		//TE_DEBUG_INFO("Applying render configs.");
		applyConfig();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//TE_DEBUG_INFO("Rendering models.");
		renderModels(scene, nullptr, MeshRenderer::RenderStage::Lit);
		
		//light pass
		fb.disable(1);
		fb.disable(2);
		fb.disable(3);
		fb.disable(4);
		fb.apply();
		//TE_DEBUG_INFO("Rendering light.");
		Light::renderLightMap(light, albedo, position, normal, specular, this, scene);
		fb.disable(5);
		fb.apply();
		//TE_DEBUG_INFO("Combining lightmap with albedomap.");
		combineLightShader.setTexture("Unlit", activeCamera->renderTexture.id());
		combineLightShader.setTexture("AlbedoMap", albedo.id());
		combineLightShader.setTexture("LightMap", light.id());
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		Render::renderShader(&combineLightShader);
		scene->postProcess(getActiveCamera()->renderTexture.id(), albedo.id(), position.id(), normal.id(), specular.id(), light.id());
	}
}