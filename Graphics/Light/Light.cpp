#include "Light.h"
#include "GameNode/ComponentFactory.h"
#include "Graphics/DeferredRenderer.h"
#include "Core.h"
#include "Kirino/Material.h"
#include <map>

namespace TealEngine
{
	Kirino::Material* pLightMaterial = nullptr;
	Kirino::Material* dLightMaterial = nullptr;
	Kirino::Material* aLightShader = nullptr;
	Kirino::Material* ssaoMaterial = nullptr;
	Kirino::Material* combineLightShader = nullptr;
	Kirino::Material* shadowMapMaterial = nullptr;
	Kirino::Material* shadowMapMaterialInstanced = nullptr;
	Kirino::Material* lightAddMaterial = nullptr;
	Kirino::Material* depthPostMaterial = nullptr;

	void lightInit()
	{
		if(pLightMaterial) delete pLightMaterial;
		if(dLightMaterial) delete dLightMaterial;
		if(shadowMapMaterial) delete shadowMapMaterial;
		if(shadowMapMaterialInstanced) delete shadowMapMaterialInstanced;
		if(ssaoMaterial) delete ssaoMaterial;
		if(lightAddMaterial) delete lightAddMaterial;
		if(depthPostMaterial) delete depthPostMaterial;
		pLightMaterial = new Kirino::Material;
		dLightMaterial = new Kirino::Material;
		shadowMapMaterial = new Kirino::Material;
		shadowMapMaterialInstanced = new Kirino::Material;
		ssaoMaterial = new Kirino::Material;
		lightAddMaterial = new Kirino::Material;
		depthPostMaterial = new Kirino::Material;
		pLightMaterial->setProgram(&Core::shadersManager.get("PointLight"));
		dLightMaterial->setProgram(&Core::shadersManager.get("DirectionLight"));
		lightAddMaterial->setProgram(&Core::shadersManager.get("LightAdd"));
		depthPostMaterial->setProgram(&Core::shadersManager.get("LightDepthPost"));

		unsigned int ditherId = Core::textureManager.get("RGBANoise.png").id();
		ssaoMaterial->setProgram(&Core::shadersManager.get("SSAO"));
		ssaoMaterial->setTexture("Dither", &ditherId);
		ssaoMaterial->setUniformBlock("CameraBlock", Core::materialsManager.getDefaultUBOs().at("CameraBlock"));

		shadowMapMaterial->setProgram(&Core::shadersManager.get("ShadowMap"));
		shadowMapMaterialInstanced->setProgram(&Core::shadersManager.get("ShadowMapInstanced"));
		shadowMapMaterial->setUniformBlock("CameraBlock", Core::materialsManager.getDefaultUBOs().at("CameraBlock"));
		shadowMapMaterial->setUniformBlock("TransformBlock", Core::materialsManager.getDefaultUBOs().at("TransformBlock"));
		shadowMapMaterialInstanced->setUniformBlock("CameraBlock", Core::materialsManager.getDefaultUBOs().at("CameraBlock"));
	}

	void DirectionLight::updateCameraPos(int i, Camera* camera)
	{
		glm::vec3 segmentCorner;
		float ratioSum = 0.0f;
		float currentZRatioSum = 0.0f;
		float currZ = camera->getNear();
		float zSegment = currZ;
		for (unsigned int j = 0; j < cascades; j++) ratioSum += this->cascadesRatio[j];

		for (unsigned int j = 0; j <= i; j++) {
			currZ = zSegment;
			zSegment += (shadowFar) * (this->cascadesRatio[j] / ratioSum);
		}
		
		glm::vec3 pos = camera->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		glm::vec4 p1 = glm::inverse(camera->getPV()) * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
		glm::vec4 p2 = glm::inverse(camera->getPV()) * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
		glm::vec4 p3 = glm::inverse(camera->getPV()) * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 p4 = glm::inverse(camera->getPV()) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;
		p4 /= p4.w;
		glm::vec3 d1 = glm::normalize(glm::vec3(p1) - pos);
		glm::vec3 d2 = glm::normalize(glm::vec3(p2) - pos);
		glm::vec3 d3 = glm::normalize(glm::vec3(p3) - pos);
		glm::vec3 d4 = glm::normalize(glm::vec3(p4) - pos);

		glm::vec3 c1 = pos + d1 * currZ;
		glm::vec3 c2 = pos + d2 * currZ;
		glm::vec3 c3 = pos + d3 * currZ;
		glm::vec3 c4 = pos + d4 * currZ;
		glm::vec3 c5 = pos + d1 * zSegment;
		glm::vec3 c6 = pos + d2 * zSegment;
		glm::vec3 c7 = pos + d3 * zSegment;
		glm::vec3 c8 = pos + d4 * zSegment;

		glm::vec3 segmentMin = 
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

		glm::vec3 segmentMax = 
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
		glm::vec3 center = (segmentMin + segmentMax) / 2.0f;//pos + camera->getNear() * forward + ((currZ + zSegment) / 2.0f) * forward * (camera->getFar() - camera->getNear());

		glm::vec3 scale = segmentMax - segmentMin;
		float scalemax = glm::max(scale.x, glm::max(scale.y, scale.z));
		camTransform.setPosition(center);
		camTransform.setRotationMatrix(((GameNode3D*)this->getParent())->getWorldTransform().getRotationMatrix());

		shadowCamera[i]->findComponentsByType<Camera>()[0]->setOrthoProjection(scalemax, scalemax, -shadowFar, scalemax);
	}

	void DirectionLight::setCascadeRatio(unsigned int cascade, float ratio)
	{
		this->cascadesRatio[cascade] = ratio;
	}

	Camera* DirectionLight::getShadowCamera(int index)
	{
		return (Camera*)(this->shadowCamera[index]->findComponentsByType<Camera>()[0]);
	}

	void DirectionLight::setupUniforms(const RenderTexturesCollection& renderTextures, glm::vec3 viewPos)
	{
		dLightMaterial->setTexture("PositionMap", &renderTextures.position, 1, 0);
		dLightMaterial->setTexture("NormalMap", &renderTextures.normal, 1, 0);
		dLightMaterial->setTexture("SpecularMap", &renderTextures.specular, 1, 0);
		dLightMaterial->setTexture("TransparentPositionMap", &renderTextures.transparentPositionAlpha, 1, 0);
		dLightMaterial->setTexture("TransparentNormalMap", &renderTextures.transparentNormal, 1, 0);
		glm::mat4 PV[8];
		unsigned int shadowMapsArray[8];
		memset(shadowMapsArray, 0, sizeof(shadowMapsArray));
		for (unsigned int i = 0; i < shadowCascades(); i++)
		{
			shadowMapsArray[i] = shadowMaps[i].id();
			PV[i] = getShadowCamera(i)->getPV();
		}

		dLightMaterial->setTexture("ShadowMap[0]", shadowMapsArray, 8, 0);

		glm::vec3 direction = ((GameNode3D*)this->getParent())->getWorldTransform().forward();
		int resolution = shadowResolution(), cascades = shadowCascades();

		dLightMaterial->setUniform("lightModel[0]", PV, sizeof(PV), 0);
		dLightMaterial->setUniform("direction", &direction, sizeof(direction), 0);
		dLightMaterial->setUniform("viewPos", &viewPos, sizeof(viewPos), 0);
		dLightMaterial->setUniform("color", &color, sizeof(color), 0);
		dLightMaterial->setUniform("indirectColor", &indirectColor, sizeof(indirectColor), 0);
		dLightMaterial->setUniform("resolution", &resolution, sizeof(int), 0);
		dLightMaterial->setUniform("cascades", &cascades, sizeof(int), 0);
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
		addProp(new ColorProp(glm::value_ptr(this->indirectColor)), "Light indirect color");
		resolution = shadow_resolution;
		this->shadowFar = shadowFar;
		this->cascades = cascades;

		for (unsigned int i = 0; i < 8; i++) {
			cascadesRatio[i] = (i*i+1.0f);
			GameNode* cameraNode = new GameNode3D();
			Camera* camera = new Camera();
			cameraNode->attachComponent(camera);
			shadowCamera.push_back(cameraNode);
			this->shadowMaps[i] = Texture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
			this->shadowMaps[i].create(resolution, resolution, 0);
			this->frameBuffers[i].attachDepthTexture(shadowMaps[i].id());
			this->frameBuffers[i].viewport(resolution, resolution);
			this->depthPostFramebuffers[i].attachTexture(shadowMaps[i].id(), 0);
			this->depthPostFramebuffers[i].enable(0);
			this->depthPostFramebuffers[i].viewport(resolution, resolution);
		}
	}
	DirectionLight::~DirectionLight()
	{
		for(int i = 0; i < 8; i++) 
		{
			delete shadowCamera[i];
		}
	}
	void DirectionLight::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer)
	{
	}

	void DirectionLight::renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) 
	{
		//render shadows
		glDepthMask(true);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glClearDepth(1.0);
		for (int c = 0; c < shadowCascades(); c++)
		{
			if((frameCounter - c) % (1 << c)) continue;
			updateCameraPos(c, camera);
			
			Core::setCamera(shadowCamera[c]->findComponentsByType<Camera>()[0]);
			frameBuffers[c].bind();
			frameBuffers[c].apply();
			glClear(GL_DEPTH_BUFFER_BIT);
			//renderer->getRendererList().render(shadowMapMaterialInstanced);
			renderer->renderModels(shadowMapMaterial);
			if(frameBuffers[c].getDepthTexture() != shadowMaps[c].id()) throw "frame buffer depth texture id does not match the shadow map texture id";
			
			float pixelsize = 1.0f / (float)resolution;
			depthPostMaterial->setUniform("pixelsize", &pixelsize, sizeof(float), 0);
			unsigned int shadowmapid = this->shadowMaps[c].id();
			depthPostMaterial->setTexture("shadowmap", &shadowmapid);
			Render::renderShader(depthPostMaterial);
			frameBuffers[c].unbind();
		}	
		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);
		glDisable(GL_CULL_FACE);
		setupUniforms(renderTextures, camera->getParentOfType<GameNode3D>()->getWorldTransform().getPosition());
		Render::renderShader(dLightMaterial);
		frameCounter++;
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
			//resize shadow maps
			frameBuffers[i].viewport(resolution, resolution);
			shadowMaps[i].create(resolution, resolution, 0);
			frameBuffers[i].attachDepthTexture(shadowMaps[i].id());
		}
	}

	PointLight::PointLight(glm::vec4 color, float radius) 
	{
		this->color = color;
		this->CLQRatios = glm::vec3(0.0f, 1.0f, 1.0f);
		addProp(new ColorProp(glm::value_ptr(this->color)), "Color");
		addProp(new FloatVecProp(glm::value_ptr(this->CLQRatios), 3), "Constant/Linear/Quadratic attenuation ratio");
		//addProp(new FloatProp(&this->radius), "Radius");
	}

	void PointLight::renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) 
	{	
		position = this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		pLightMaterial->setTexture("PositionMap", &renderTextures.position, 1, 0);
		pLightMaterial->setTexture("NormalMap", &renderTextures.normal, 1, 0);
		pLightMaterial->setTexture("SpecularMap", &renderTextures.specular, 1, 0);
		glm::vec3 viewPos = camera->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
		pLightMaterial->setUniform("viewPos", &viewPos, sizeof(float) * 3, 0);
		pLightMaterial->setUniform("color", &color, sizeof(float) * 4, 0);
		//pLightMaterial->setUniform("radius", &radius, sizeof(float) * 1, 0);
		pLightMaterial->setUniform("CLQRatios", &CLQRatios, sizeof(float) * 3, 0);
		Render::renderShader(pLightMaterial);
	}

	SSAO::SSAO() : ssaoBufferTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT)
	{
		addProp(new FloatProp(&strength), "Strength");
		addProp(new FloatProp(&radius), "Radius");
		addProp(new ColorProp(glm::value_ptr(ambient)), "Ambient");
		strength = 0.5f;
		radius = 0.1f;

		ssaoBufferTexture.create(1, 1, 0);
		
		fbo.bind();
		fbo.attachTexture(ssaoBufferTexture.id(), 0);
		fbo.enable(1);
		fbo.apply();
		fbo.unbind();
	}
	
	void SSAO::renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) 
	{

		fbo.bind();
		fbo.apply();
		//glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT);
		int w = camera->getWidth() / 4.0f;
		int h = camera->getHeight() / 4.0f;
		if(ssaoBufferTexture.getWidth() != w || ssaoBufferTexture.getHeight() != h) 
		{
			fbo.viewport(w, h);
			ssaoBufferTexture.create(w, h);
		} 
		ssaoMaterial->setTexture("PositionMap", &renderTextures.position, 1, 0);
		ssaoMaterial->setTexture("NormalMap", &renderTextures.normal, 1, 0);
		glm::vec3 dir = camera->getParentOfType<GameNode3D>()->getWorldTransform().forward();
		ssaoMaterial->setUniform("direction", &dir, sizeof(float) * 3, 0);
		ssaoMaterial->setUniform("strength", &strength, sizeof(float), 0);
		ssaoMaterial->setUniform("radius", &radius, sizeof(float), 0);
		ssaoMaterial->setUniform("ambient", &ambient, sizeof(float) * 4, 0);
		Render::renderShader(ssaoMaterial);
		//glEnable(GL_BLEND);
		fbo.unbind();
		unsigned int ssaoTexId = ssaoBufferTexture.id();
		lightAddMaterial->setTexture("textureImage", &ssaoTexId, 1, 0);
		Render::renderShader(lightAddMaterial);
	}

	EXPORT_COMPONENT(SSAO);
	EXPORT_COMPONENT(DirectionLight);
	EXPORT_COMPONENT(PointLight);
}