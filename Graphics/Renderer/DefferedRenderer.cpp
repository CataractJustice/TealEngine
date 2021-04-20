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
		if(position.id() == 0)position = normal = Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT);
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

		empty3D = Resources::getShader("empty_3d");
	}
	void DefferedRenderer::render()
	{
		//render pass
		fb.bind();
		fb.attachTexture(activeCamera->renderTexture.id(), 0);
		//fb.enable(0);
		fb.enable(1);
		fb.enable(2);
		fb.enable(3);
		fb.enable(4);
		fb.enable(5);
		fb.apply();
		applyConfig();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		renderModels();
		
		//light pass
		fb.disable(1);
		fb.disable(2);
		fb.disable(3);
		fb.disable(4);
		fb.apply();
		renderLights();
		fb.disable(5);
		combineLightShader.setTexture("AlbedoMap", albedo.id());
		combineLightShader.setTexture("LightMap", light.id());
		Render::renderShader(&combineLightShader);
	}


	void DefferedRenderer::renderLights()
	{
		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			LightType lightType = (*it)->getLightType();
			AmbientLight* ambientLight = (AmbientLight*)(*it);
			DirectionLight* directionLight = (DirectionLight*)(*it);
			switch (lightType)
			{
			case TealEngine::AMBIENT_LIGHT:
				fb.bind();
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				ambientLight->setupUniforms(light.id(), albedo.id(), position.id(), normal.id());
				Render::renderShader(&aLightShader);
				break;

			case TealEngine::DIRECTION_LIGHT:
				directionLight->updateProjections(this->activeCamera);
				//render shadow maps
				shadowMapRenderer.fb.viewport(directionLight->shadowResolution(), directionLight->shadowResolution());
				shadowMapRenderer.fb.bind();
				for (unsigned int i = 0; i < directionLight->shadowCascades(); i++)
				{
					shadowMapRenderer.setCamera(directionLight->getShadowCamera(i));
					shadowMapRenderer.fb.attachDepthTexture(directionLight->getShadowCamera(i)->renderTexture.id());
					glCullFace(GL_FRONT);
					shadowMapRenderer.render(this->meshList, &empty3D, true);
				}

				//render light to camera texture
				fb.bind();
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				directionLight->setupUniforms(light.id(), albedo.id(), position.id(), normal.id(), specular.id(), activeCamera->getWorldTransform().getPosition());
				Render::renderShader(&dLightShader);
				break;

			case TealEngine::SPOT_LIGHT:
				break;

			case TealEngine::POINT_LIGHT:
				break;

			default:
				TE_DEBUG_WARNING("Invalid light type.");
				break;
			}
		}
	}

	void DefferedRenderer::pushLight(Light* light)
	{
		this->lights.push_back(light);
		this->push((GameNode*)light);
	}

	void DefferedRenderer::popLight(Light* light)
	{
		this->lights.remove(light);
	}

	void DefferedRenderer::push(MeshRenderer* mesh) 
	{
		this->meshList.push_back(mesh);
	}

	void DefferedRenderer::push(GameNode* scene)
	{
		vector<GameNode*> s = scene->getChilds();
		for (GameNode* node : s)
		{
			if (dynamic_cast<MeshRenderer*>(node)) push((MeshRenderer*)node);
			if (dynamic_cast<Light*>(node)) pushLight((Light*)node);
			push(node);
		}
	}
}