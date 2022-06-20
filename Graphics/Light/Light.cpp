#include "Light.h"
#include "Graphics/Renderer/DefferedRenderer.h"

namespace TealEngine
{
	ShaderProgram dLightShader;
	ShaderProgram aLightShader;
	ShaderProgram combineLightShader;
	ShaderProgram shadowMapShader;

	std::vector<Light*> Light::lights;
	ForwardRenderer Light::shadowMapRenderer;
	FrameBuffer Light::fb;

	Light::Light() {
		lights.push_back(this);
		color = vec4(1.0f);
	}

	void lightInit()
	{
		dLightShader = Resources::getShader("direction_light");
		aLightShader = Resources::getShader("ambient_light");
		combineLightShader = Resources::getShader("light_combine");
		shadowMapShader = Resources::getShader("empty_3d");
	}

	void Light::renderLightMap(Texture& light, Texture& albedo, Texture& position, Texture& normal, Texture& specular, DefferedRenderer* renderer)
	{
		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			Light* lightsource = (Light*)(*it);
			lightsource->render(light, albedo, position, normal, specular, renderer);
		}
	}

	void DirectionLight::updateCameraPos(int i, Camera* camera)
	{
		vec3 front = camera->getRelativeTransform().forward();
		vec3 segmentCorner;
		float ratioSum = 0.0f;
		float currentZRatioSum = 0.0f;
		float currZ = camera->getNear();
		float zSegment = camera->getNear();
		for (unsigned int j = 0; j < cascades; j++) ratioSum += this->cascadesRatio[j];

		for (unsigned int j = 0; j <= i; j++) {
			currZ = zSegment;
			zSegment = (shadowFar - camera->getNear()) / ratioSum * this->cascadesRatio[j] + currZ;
		}

		vec3 segmentMin = vec3(numeric_limits<float>::max()), segmentMax = vec3(-numeric_limits<float>::max());
		
		for (int j = 0; j < 8; j++)
		{
			//calc segment corner in camera coordinate system
			//calc segment corner in global coordinate system
			segmentCorner = glm::inverse(camera->getPV()) * vec4((j % 2) ? 1.0f : -1.0f, ((j / 2) % 2) ? 1.0f : -1.0f, ((j >= 4) ? currZ : zSegment), 1.0f) / (camera->getFar() - camera->getNear());
			//calc segment corner in sun coordinate system
			segmentCorner = ((GameNode3D*)this->getParrent())->getWorldTransform().getMatrix() * glm::vec4(segmentCorner, 1.0f);

			segmentMin = vec3(glm::min(segmentCorner.x, segmentMin.x), glm::min(segmentCorner.y, segmentMin.y), glm::min(segmentCorner.z, segmentMin.z));
			segmentMax = vec3(glm::max(segmentCorner.x, segmentMax.x), glm::max(segmentCorner.y, segmentMax.y), glm::max(segmentCorner.z, segmentMax.z));
		}
		Transform camTransform = ((GameNode3D*)this->getParrent())->getRelativeTransform();
		camTransform.setPosition(camera->getWorldTransform().getPosition() + camera->getWorldTransform().forward() * (currZ + zSegment) / 2.0f);
		vec3 center = (segmentMax + segmentMin) / 2.0f;
		vec3 scale = (segmentMax - segmentMin);
		scale = vec3(glm::max(glm::max(scale.x, scale.y), scale.z));

		((Camera*)(shadowCamera[i]))->setOrthoProjection(scale.x * 2.4f, scale.y * 2.4f, -shadowFar, scale.z);
		((Camera*)(shadowCamera[i]))->setWorldTransform(camTransform);
	}

	void DirectionLight::setCascadeRatio(unsigned int cascade, float ratio)
	{
		this->cascadesRatio[cascade] = ratio;
	}

	Camera* DirectionLight::getShadowCamera(int index)
	{
		return (Camera*)this->shadowCamera[index];
	}

	void DirectionLight::setupUniforms(GLuint render, GLuint albedo, GLuint position, GLuint normal, GLuint specular, vec3 viewPos)
	{
		dLightShader.setTexture("RenderTexture", render);
		//dLightShader.setTexture("AlbedoMap", albedo);
		dLightShader.setTexture("PositionMap", position);
		dLightShader.setTexture("NormalMap", normal);
		dLightShader.setTexture("SpecularMap", specular);
		mat4 PV[8];
		for (unsigned int i = 0; i < shadowCascades(); i++)
		{
			dLightShader.setTexture("ShadowMap", i, getShadowCamera(i)->renderTexture.id());
			PV[i] = getShadowCamera(i)->getPV();
		}


		vec3 direction = ((GameNode3D*)this->getParrent())->getWorldTransform().forward();
		int resolution = shadowResolution(), cascades = shadowCascades();

		dLightShader.setUniform("lightModel", PV, cascades);
		dLightShader.setUniform("direction", direction);
		dLightShader.setUniform("viewPos", viewPos);
		dLightShader.setUniform("color", color);
		dLightShader.setUniform("resolution", resolution);
		dLightShader.setUniform("cascades", cascades);
	}

	GLuint DirectionLight::shadowResolution()
	{
		return this->resolution;
	}

	GLuint DirectionLight::shadowCascades()
	{
		return this->cascades;
	}

	DirectionLight::DirectionLight(GLuint shadow_resolution, GLuint cascades, GLfloat shadowFar)
	{
		resolution = shadow_resolution;
		this->shadowFar = shadowFar;
		this->cascades = cascades;
		for (unsigned int i = 0; i < cascades; i++) {
			cascadesRatio[i] = 1.0f;
			shadowCamera.push_back(new Camera());
			((Camera*)shadowCamera[i])->renderTexture = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
			((Camera*)shadowCamera[i])->renderTexture.create(shadow_resolution, shadow_resolution);
			((Camera*)shadowCamera[i])->renderTexture.setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			((Camera*)shadowCamera[i])->renderTexture.setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		
	}

	void DirectionLight::render(Texture& light, Texture& albedo, Texture& position, Texture& normal, Texture& specular, DefferedRenderer* renderer)
	{
		shadowMapRenderer.setDepthClear(true);
		shadowMapRenderer.setDepthTest(true);

		for (int c = 0; c < shadowCascades(); c++)
		{
			updateCameraPos(c, renderer->getActiveCamera());
			shadowMapRenderer.fb.resize(shadowResolution(), shadowResolution());
			shadowMapRenderer.fb.bind();
			for (unsigned int i = 0; i < cascades; i++)
			{
					shadowMapRenderer.setCamera(getShadowCamera(i));
					shadowMapRenderer.fb.attachDepthTexture(getShadowCamera(i)->renderTexture.id());
					shadowMapRenderer.fb.bind();
					shadowMapRenderer.render(renderer->getMeshRendererList(), &shadowMapShader, true);
			}
		}
		//TE_DEBUG_INFO("Render direction light to light map");
		//render light to camera texture
		renderer->fb.bind();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		setupUniforms(light.id(), albedo.id(), position.id(), normal.id(), specular.id(), renderer->getActiveCamera()->getWorldTransform().getPosition());
		Render::renderShader(&dLightShader);
	}

	/*
	class AmbientLight : public Light
	{
	public:
		vec4 color;
		int SSAOarea;
		float SSAOstrength;
		LightType getLightType() override
		{
			return AMBIENT_LIGHT;
		}
		void setupUniforms(GLuint render, GLuint albedo, GLuint position, GLuint normal)
		{
			aLightShader.setTexture("RenderMap", render);
			aLightShader.setTexture("AlbedoMap", albedo);
			aLightShader.setTexture("PositionMap", position);
			aLightShader.setTexture("NormalMap", normal);

			aLightShader.setUniform("VPmat", Render::VP_matrix);
			aLightShader.setUniform("color", color);
			aLightShader.setUniform("resolution", vec2(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight()));
			aLightShader.setUniform("ssaoarea", SSAOarea);
			aLightShader.setUniform("ssaostrength", SSAOstrength);
		}
	};
	*/
}