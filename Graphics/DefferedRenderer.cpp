#include "DeferredRenderer.h"
#include "Graphics/Light/Light.h"
#include "Core.h"
#include "Kirino/UniformBufferObject.h"
#include "System/VTable.h"

namespace TealEngine
{
	void DeferredRenderer::onSceneComponentAdd(Component* component, GameNode* parent) 
	{
		Mesh3DRendererInstanced* rendererInstanced = dynamic_cast<Mesh3DRendererInstanced*>(component);
		if(rendererInstanced) 
		{
			this->renderList.addMeshRenderer(rendererInstanced);
			return;
		}

		if(isMethodOverriden(Component::base, component, &Component::render))
		{
			renderers.emplace(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::postRender)) 
		{
			postRenderers.emplace(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::shadowRender)) 
		{
			shadowRenderers.emplace(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::transparentRender)) 
		{
			transparentRenderers.emplace(component);
		}

		ILight* light = dynamic_cast<ILight*>(component);
		if(light) 
		{
			this->lightsSet.insert(light);
		}
	}
	void DeferredRenderer::onSceneComponentRemove(Component* component, GameNode* parent) 
	{
		Mesh3DRendererInstanced* rendererInstanced = dynamic_cast<Mesh3DRendererInstanced*>(component);
		if(rendererInstanced) 
		{
			this->renderList.removeMeshRenderer(rendererInstanced);
			return;
		}

		if(isMethodOverriden(Component::base, component, &Component::render))
		{
			renderers.erase(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::postRender)) 
		{
			postRenderers.erase(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::shadowRender)) 
		{
			shadowRenderers.erase(component);
		}
		if(isMethodOverriden(Component::base, component, &Component::transparentRender)) 
		{
			transparentRenderers.erase(component);
		}

		ILight* light = dynamic_cast<ILight*>(component);
		if(light) 
		{
			this->lightsSet.erase(light);
		}
	}

	void DeferredRenderer::resize(unsigned int width, unsigned int height) 
	{
		fb.viewport(width, height);
		depth.create(width, height);
		unlit.create(width, height);
		color.create(width, height);
		position.create(width, height);
		normal.create(width, height);
		specular.create(width, height);
		light.create(width, height);
		transparentNormal.create(width, height);
		transparentPositionAlpha.create(width, height);
	}
	
	DeferredRenderer::DeferredRenderer() 
	{
		renderList.init();
		int width = 640;
		int height = 480;

		depth = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
		position = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
		normal = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
		transparentNormal = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
		transparentPositionAlpha = Texture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT);
		specular = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
		light = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);

		resize(width, height);

		fb.bind();
		fb.attachDepthTexture(depth.id());
		fb.attachTexture(unlit.id(), 0);
		fb.attachTexture(color.id(), 1);
		fb.attachTexture(position.id(), 2);
		fb.attachTexture(normal.id(), 3);
		fb.attachTexture(specular.id(), 4);
		fb.attachTexture(light.id(), 5);
		fb.attachTexture(transparentPositionAlpha.id(), 6);
		fb.attachTexture(transparentNormal.id(), 7);
		fb.enable(0);
		fb.enable(1);
		fb.enable(2);
		fb.enable(3);
		fb.enable(4);
		fb.enable(5);
		fb.enable(6);
		fb.enable(7);
		fb.apply();
		fb.unbind();

		unsigned int unlitId = unlit.id();
		unsigned int albedoId = color.id();
		unsigned int lightId = light.id();
		unsigned int ditherId = Core::textureManager.get("RGBNoise.png").id();

		lightCombineMaterial.setProgram(&Core::shadersManager.get("LightCombine"));
		lightCombineMaterial.setTexture("UnlitMap", &unlitId, 1, 0);
		lightCombineMaterial.setTexture("AlbedoMap", &albedoId, 1, 0);
		lightCombineMaterial.setTexture("LightMap", &lightId, 1, 0);
		lightCombineMaterial.setTexture("Dither", &ditherId, 1, 0);
		lightCombineMaterial.setupUniforms();
	}

	void DeferredRenderer::render(Camera* camera) 
	{
		lightRenderingState = false;
		if(camera == nullptr) return;
		Core::setCamera(camera);
		if(camera->getWidth() != this->color.getWidth()) resize(camera->getWidth(), camera->getHeight());	
		fb.bind();
		//render models
		glDepthMask(true);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
		
		Profiler::ProfilerPoint renderListProf(&Core::profiler, "Render models");
		Profiler::ProfilerPoint updListProf(&Core::profiler, "Update list");
		renderList.updateList();
		updListProf.end();
		Profiler::ProfilerPoint updMatricesProf(&Core::profiler, "Update matrices");
		renderList.uploadDirtyMatrices();
		updMatricesProf.end();
		renderList.render();
		renderModels();
		renderTransparentModels();
		renderListProf.end();

		//render and blend lights
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		RenderTexturesCollection renderTextures;
		renderTextures.color = this->color.id();
		renderTextures.depth = this->depth.id();
		renderTextures.normal = this->normal.id();
		renderTextures.position = this->position.id();
		renderTextures.specular = this->specular.id();
		renderTextures.unlit = this->unlit.id();
		renderTextures.transparentPositionAlpha = this->transparentPositionAlpha.id();
		renderTextures.transparentNormal = this->transparentNormal.id();
		
		Profiler::ProfilerPoint renderLightProf(&Core::profiler, "Render lights");
		lightRenderingState = true;
		for(ILight* light : this->lightsSet) 
		{
			if(light->isActive()) 
			{
				light->renderLight(renderTextures, camera, this);
			}
		}
		lightRenderingState = false;
		renderLightProf.end();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthMask(true);
		Core::setCamera(camera);
		renderPostModels();
		fb.unbind();
		glDepthMask(false);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		//apply lighting
		Render::renderShader(&lightCombineMaterial);
	}

	void DeferredRenderer::onWindowResize(int width, int height) 
	{
		this->resize(width, height);
	}
	
	void DeferredRenderer::renderModels(Kirino::Material* material) 
	{
		RenderTexturesCollection renderTextures;
		renderTextures.color = this->color.id();
		renderTextures.depth = this->depth.id();
		renderTextures.normal = this->normal.id();
		renderTextures.position = this->position.id();
		renderTextures.specular = this->specular.id();
		renderTextures.unlit = this->unlit.id();
		renderTextures.transparentPositionAlpha = this->transparentPositionAlpha.id();
		renderTextures.transparentNormal = this->transparentNormal.id();
		stateCollection.textureBindings.reset();
		stateCollection.uboBindings.reset();
		stateCollection.vao = -1;
		stateCollection.usedMaterial = nullptr;
		stateCollection.forcedMaterial = material;
		stateCollection.shader = material ? material->getShaderProgram() : nullptr;
		if(stateCollection.forcedMaterial) 
		{
			stateCollection.forcedMaterial->useProgram();
			stateCollection.forcedMaterial->setupTextures(stateCollection.textureBindings);
			stateCollection.forcedMaterial->setupUniforms();
			stateCollection.forcedMaterial->setupUniformBlocks(stateCollection.uboBindings);
			stateCollection.usedMaterial = stateCollection.forcedMaterial;
		}

		FrustumPlanes frustum = Core::camera->getFrustumPlanes();
		glm::vec3 cameraPosition = Core::camera->getParentOfTypeConst<GameNode3D>()->getWorldTransform().getPosition();
		
		for(Component* renderable : renderers) 
		{
			if(!lightRenderingState) 
			{
				renderable->render(stateCollection, frustum, renderTextures);
			}
			else 
			{
				renderable->shadowRender(stateCollection, frustum, renderTextures);
			}
			if(stateCollection.forcedMaterial && stateCollection.usedMaterial != stateCollection.forcedMaterial) 
			{
				stateCollection.forcedMaterial->useProgram();
				stateCollection.forcedMaterial->setupTextures(stateCollection.textureBindings);
				stateCollection.forcedMaterial->setupUniforms();
				stateCollection.forcedMaterial->setupUniformBlocks(stateCollection.uboBindings);
				stateCollection.usedMaterial = stateCollection.forcedMaterial;
			}
		}
	}

	void DeferredRenderer::renderPostModels() 
	{
		RenderTexturesCollection renderTextures;
		renderTextures.color = this->color.id();
		renderTextures.depth = this->depth.id();
		renderTextures.normal = this->normal.id();
		renderTextures.position = this->position.id();
		renderTextures.specular = this->specular.id();
		renderTextures.unlit = this->unlit.id();
		renderTextures.transparentPositionAlpha = this->transparentPositionAlpha.id();
		renderTextures.transparentNormal = this->transparentNormal.id();
		stateCollection.textureBindings.reset();
		stateCollection.uboBindings.reset();
		stateCollection.vao = -1;
		stateCollection.usedMaterial = nullptr;
		stateCollection.forcedMaterial = nullptr;
		stateCollection.shader = nullptr;

		FrustumPlanes frustum = Core::camera->getFrustumPlanes();
		glm::vec3 cameraPosition = Core::camera->getParentOfTypeConst<GameNode3D>()->getWorldTransform().getPosition();
		
		for(Component* renderable : postRenderers) 
		{
			renderable->postRender(stateCollection, frustum, renderTextures);
		}
	}

	void DeferredRenderer::renderTransparentModels() 
	{
		RenderTexturesCollection renderTextures;
		renderTextures.color = this->color.id();
		renderTextures.depth = this->depth.id();
		renderTextures.normal = this->normal.id();
		renderTextures.position = this->position.id();
		renderTextures.specular = this->specular.id();
		renderTextures.unlit = this->unlit.id();
		renderTextures.transparentPositionAlpha = this->transparentPositionAlpha.id();
		renderTextures.transparentNormal = this->transparentNormal.id();
		stateCollection.textureBindings.reset();
		stateCollection.uboBindings.reset();
		stateCollection.vao = -1;
		stateCollection.usedMaterial = nullptr;
		stateCollection.forcedMaterial = nullptr;
		stateCollection.shader = nullptr;

		FrustumPlanes frustum = Core::camera->getFrustumPlanes();
		glm::vec3 cameraPosition = Core::camera->getParentOfTypeConst<GameNode3D>()->getWorldTransform().getPosition();
		
		for(Component* renderable : transparentRenderers) 
		{
			renderable->transparentRender(stateCollection, frustum, renderTextures);
		}
	}
} // namespace TealEngine
