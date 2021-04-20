#pragma once
#include "../../GameNode/GameNode.h"
#include "../Camera.h"
#include "../Mesh/MeshUtil.h"
#include "Resources.h"
#include "../../Graphics/Renderer/RenderUtil.h"
#include <limits>

namespace TealEngine 
{
	extern ShaderProgram dLightShader;
	extern ShaderProgram aLightShader;
	extern ShaderProgram combineLightShader;
	extern void lightInit();

	enum LightType
	{
		AMBIENT_LIGHT,
		DIRECTION_LIGHT,
		SPOT_LIGHT,
		POINT_LIGHT
	};

	class Light : public GameNode3D 
	{
	private:
	public:
		vec4 color;
		virtual LightType getLightType() = 0;
	};

	class DirectionLight : public Light 
	{
	private:
		GLuint resolution;
		GLfloat shadowFar;
		vector<GameNode*> shadowCamera;
		GLuint cascades;
		float cascadesRatio[8];
		
	public:
		
		LightType getLightType() override 
		{
			return DIRECTION_LIGHT;
		}

		void setCascadeRatio(unsigned int cascade, float ratio) 
		{
			this->cascadesRatio[cascade] = ratio;
		}

		Camera* getShadowCamera(int index) 
		{
			return (Camera*)this->shadowCamera[index];
		}

		void updateProjections(Camera* camera) 
		{
			
			vec3 front = camera->getTransform().forward();
			vec3 segmentCorner;
			float ratioSum = 0.0f;
			float currZ = camera->getNear();
			float zSegment;
			for (unsigned int i = 0; i < cascades; i++) ratioSum += this->cascadesRatio[i];
			//float minSegment = (camera->getFar() - camera->getNear()) * (float(cascades)-1.0f) / (pow(2.0f, float(cascades))-1.0f);
			for (unsigned int i = 0; i < this->cascades; i++) 
			{
				
				vec3 segmentMin = vec3(numeric_limits<float>::max()), segmentMax = vec3(-numeric_limits<float>::max());
				for (int j = 0; j < 8; j++)
				{
					zSegment = (shadowFar - camera->getNear()) / ratioSum * cascadesRatio[i] * float(j > 3) + currZ;
					//calc segment corner in camera coordinate system
					segmentCorner = vec3((-1.0f + float(j % 2) * 2.0f) * (zSegment / tan(camera->getFOV() / 2.0f)), (-1.0f + float((j % 4) > 1) * 2.0f) * (zSegment / tan(camera->getFOV() / 2.0f)), zSegment);
					//calc segment corner in global coordinate system
					segmentCorner = camera->getWorldTransform().right() * segmentCorner.x + camera->getWorldTransform().up() * segmentCorner.y + camera->getWorldTransform().forward() * segmentCorner.z + camera->getWorldTransform().getPosition();
					//calc segment corner in sun coordinate system
					segmentCorner -= this->getWorldTransform().getPosition();
					segmentCorner = vec3(dot(this->transform.right(), segmentCorner), dot(this->transform.up(), segmentCorner), dot(this->transform.forward(), segmentCorner));
					
					segmentMin = vec3(glm::min(segmentCorner.x, segmentMin.x), glm::min(segmentCorner.y, segmentMin.y), glm::min(segmentCorner.z, segmentMin.z));
					segmentMax = vec3(glm::max(segmentCorner.x, segmentMax.x), glm::max(segmentCorner.y, segmentMax.y), glm::max(segmentCorner.z, segmentMax.z));
				}
				currZ = zSegment;
				vec3 center = (segmentMax + segmentMin) /2.0f;
				vec3 scale = (segmentMax - segmentMin);
				scale = vec3(glm::max(glm::max(scale.x, scale.y), scale.z));

				((Camera*)(shadowCamera[i]))->setOrthoProjection(scale.x*1.2f, scale.y*1.2f, -camera->getFar()*2.0f, scale.z);
				Transform camTransform = ((Camera*)(shadowCamera[i]))->getTransform();
				camTransform.setPosition(center);
				((Camera*)(shadowCamera[i]))->setRelativeTransform(camTransform);
			}
		}

		void setupUniforms(GLuint render, GLuint albedo, GLuint position, GLuint normal, GLuint specular, vec3 viewPos) 
		{
			dLightShader.setTexture("RenderMap", render);
			dLightShader.setTexture("AlbedoMap", albedo);
			dLightShader.setTexture("PositionMap", position);
			dLightShader.setTexture("NormalMap", normal);
			dLightShader.setTexture("SpecularMap", specular);
			mat4 PV[8];
			for (unsigned int i = 0; i < shadowCascades(); i++)
			{
				dLightShader.setTexture("ShadowMap", i, getShadowCamera(i)->renderTexture.id());
				PV[i] = getShadowCamera(i)->getPV();
			}


			vec3 direction = getWorldTransform().forward();
			int resolution = shadowResolution(), cascades = shadowCascades();

			dLightShader.setUniform("lightModel", PV[0], cascades);
			dLightShader.setUniform("direction", direction);
			dLightShader.setUniform("viewPos", viewPos);
			dLightShader.setUniform("color", color);
			dLightShader.setUniform("resolution", resolution);
			dLightShader.setUniform("cascades", cascades);
		}

		GLuint shadowResolution() 
		{
			return this->resolution;
		}

		GLuint shadowCascades() 
		{
			return this->cascades;
		}
		DirectionLight(GLuint shadow_resolution, GLuint cascades = 3U, GLfloat shadowFar = 250.0f) 
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
				//((Camera*)shadowCamera[i])->renderTexture.setBorderColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
				this->addChild(shadowCamera[i]);
			}
		}
	};


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
}