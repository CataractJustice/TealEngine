#include "Light.h"
#include "Graphics/Renderer/DeferredRenderer.h"
#include "GameNode/ComponentFactory.h"
#include "Core.h"
#include <map>

namespace TealEngine
{
	ShaderProgram pLightShader;
	ShaderProgram dLightShader;
	ShaderProgram aLightShader;
	ShaderProgram combineLightShader;
	ShaderProgram shadowMapShader;
	ForwardRenderer shadowMapRenderer;
	
	std::map<std::string, ShaderProgram> lightShaders;

	void lightInit()
	{
		ShaderProgram::loadShadersFromJson(lightShaders, "./Assets/Shaders/LightShaders.json");
		ShaderProgram::loadShadersFromJson(lightShaders, "./Assets/Shaders/DepthShader.json");
		dLightShader 		=	lightShaders["DirectionLight"];
		pLightShader 		=	lightShaders["PointLight"];
		combineLightShader 	= 	lightShaders["LightCombine"];
		shadowMapShader 	= 	lightShaders["Depth"];
	}

	void DirectionLight::updateCameraPos(int i, Camera* camera)
	{
		vec3 segmentCorner;
		float ratioSum = 0.0f;
		float currentZRatioSum = 0.0f;
		float currZ = camera->getNear();
		float zSegment = currZ;
		for (unsigned int j = 0; j < cascades; j++) ratioSum += this->cascadesRatio[j];

		for (unsigned int j = 0; j <= i; j++) {
			currZ = zSegment;
			zSegment += (shadowFar) * (this->cascadesRatio[j] / ratioSum);
		}
		
		vec3 pos = camera->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		vec4 p1 = glm::inverse(camera->getPV()) * vec4(-1.0f, -1.0f, 1.0f, 1.0f);
		vec4 p2 = glm::inverse(camera->getPV()) * vec4(1.0f, -1.0f, 1.0f, 1.0f);
		vec4 p3 = glm::inverse(camera->getPV()) * vec4(-1.0f, 1.0f, 1.0f, 1.0f);
		vec4 p4 = glm::inverse(camera->getPV()) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;
		p4 /= p4.w;
		vec3 d1 = glm::normalize(vec3(p1) - pos);
		vec3 d2 = glm::normalize(vec3(p2) - pos);
		vec3 d3 = glm::normalize(vec3(p3) - pos);
		vec3 d4 = glm::normalize(vec3(p4) - pos);

		vec3 c1 = pos + d1 * currZ;
		vec3 c2 = pos + d2 * currZ;
		vec3 c3 = pos + d3 * currZ;
		vec3 c4 = pos + d4 * currZ;
		vec3 c5 = pos + d1 * zSegment;
		vec3 c6 = pos + d2 * zSegment;
		vec3 c7 = pos + d3 * zSegment;
		vec3 c8 = pos + d4 * zSegment;

		vec3 segmentMin = 
		glm::min(
			glm::min(
				glm::min(c1, c2),
				glm::min(c3, c4)
			),
			glm::min(
				glm::min(c5, c6),
				glm::min(c7, c8)
			)
		);

		vec3 segmentMax = 
		glm::max(
			glm::max(
				glm::max(c1, c2),
				glm::max(c3, c4)
			),
			glm::max(
				glm::max(c5, c6),
				glm::max(c7, c8)
			)
		);

		Transform& camTransform = ((GameNode3D*)shadowCamera[i])->getRelativeTransform();
		vec3 center = (segmentMin + segmentMax) / 2.0f;//pos + camera->getNear() * forward + ((currZ + zSegment) / 2.0f) * forward * (camera->getFar() - camera->getNear());

		vec3 scale = vec3(glm::length(segmentMax - segmentMin));
		camTransform.setPosition(center);
		camTransform.setRotationMatrix(((GameNode3D*)this->getParent())->getWorldTransform().getRotationMatrix());

		shadowCamera[i]->findComponentsByType<Camera>()[0]->setOrthoProjection(scale.x, scale.y, -shadowFar, scale.z);
	}

	void DirectionLight::setCascadeRatio(unsigned int cascade, float ratio)
	{
		this->cascadesRatio[cascade] = ratio;
	}

	Camera* DirectionLight::getShadowCamera(int index)
	{
		return (Camera*)(this->shadowCamera[index]->findComponentsByType<Camera>()[0]);
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


		vec3 direction = ((GameNode3D*)this->getParent())->getWorldTransform().forward();
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
		addProp(new IntProp((int*)&this->resolution), "Shadow map resolution");
		addProp(new FloatProp((float*)&this->shadowFar), "Shadow distance");
		addProp(new IntProp((int*)&this->cascades), "Count of cascades");
		addProp(new ColorProp(glm::value_ptr(this->color)), "Light color");
		resolution = shadow_resolution;
		this->shadowFar = shadowFar;
		this->cascades = cascades;
		for (unsigned int i = 0; i < 8; i++) {
			cascadesRatio[i] = (i*i+1.0f) * (i*i+1.0f);
			GameNode* cameraNode = new GameNode3D();
			Camera* camera = new Camera();
			cameraNode->attachComponent(camera);
			shadowCamera.push_back(cameraNode);
			camera->renderTexture = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
			camera->renderTexture.create(resolution, resolution);
			camera->renderTexture.setParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			camera->renderTexture.setParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		
	}

	void DirectionLight::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light)
	{
		shadowMapRenderer.setDepthClear(true);
		shadowMapRenderer.setDepthTest(true);

		for (int c = 0; c < shadowCascades(); c++)
		{
			updateCameraPos(c, Core::renderer.getActiveCamera());
			shadowMapRenderer.fb.resize(shadowResolution(), shadowResolution());
			shadowMapRenderer.fb.bind();
			shadowMapRenderer.setCamera(getShadowCamera(c));
			shadowMapRenderer.fb.attachDepthTexture(getShadowCamera(c)->renderTexture.id());
			shadowMapRenderer.fb.bind();
			shadowMapRenderer.render(Core::getRoot(), &shadowMapShader, true);
		}
		//TE_DEBUG_INFO("Render direction light to light map");
		//render light to camera texture
		Core::renderer.fb.bind();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		setupUniforms(light, unlitColor, position, normal, specular, Core::renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform().getPosition());
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

	void DirectionLight::onPropSet(const std::string& name) 
	{
		if(cascades > 8) cascades = 8;
		if(cascades < 1) cascades = 1;
		if(resolution < 1) resolution = 1;
		//resolution cap is not necessary but is implemented to prevent accidental crashes from too extreeme values
		if(resolution > 8192) resolution = 8192;
		for (unsigned int i = 0; i < cascades; i++) {
			Camera* camera = shadowCamera[i]->findComponentsByType<Camera>()[0];
			camera->renderTexture.create(resolution, resolution);
		}
	}

	PointLight::PointLight(glm::vec4 color, float radius) 
	{
		this->color = color;
		this->radius = radius;
		addProp(new ColorProp(glm::value_ptr(this->color)), "Color");
		addProp(new FloatProp(&this->radius), "Radius");
	}

	void PointLight::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light)
	{
		Core::renderer.fb.bind();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		pLightShader.setTexture("RenderTexture", light);
		pLightShader.setTexture("PositionMap", position);
		pLightShader.setTexture("NormalMap", normal);
		pLightShader.setTexture("SpecularMap", specular);


		vec3 lightPos = ((GameNode3D*)this->getParent())->getWorldTransform().getPosition();
		vec3 viewPos = Core::renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();

		pLightShader.setUniform("position", lightPos);
		pLightShader.setUniform("viewPos", viewPos);
		pLightShader.setUniform("color", color);
		pLightShader.setUniform("radius", radius);

		Render::renderShader(&pLightShader);
	}

	EXPORT_COMPONENT(DirectionLight);
	EXPORT_COMPONENT(PointLight);
}