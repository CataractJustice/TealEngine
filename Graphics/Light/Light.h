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
	public:
		enum DIRECTION_LIGHT_CASCADER_DRAW_ORDER
		{
			ALL_CASCADES_AT_ONCE,
			ONE_CASCADE_PER_FRAME,
			PRIORITIZE_CLOSEST
		};

	private:
		GLuint resolution;
		GLfloat shadowFar;
		vector<GameNode*> shadowCamera;
		GLuint cascades;
		float cascadesRatio[8];
		
		unsigned char cascadeCounter;
		DIRECTION_LIGHT_CASCADER_DRAW_ORDER drawOrder;

		void updateCameraPos(int i, float& ratioSum, float& currZ, float& zSegment, glm::vec3& segmentCorner, Camera* camera)
		{
			vec3 segmentMin = vec3(numeric_limits<float>::max()), segmentMax = vec3(-numeric_limits<float>::max());
			zSegment = (shadowFar - camera->getNear()) / ratioSum * cascadesRatio[i] + currZ;
			for (int j = 0; j < 8; j++)
			{
				//calc segment corner in camera coordinate system
				//calc segment corner in global coordinate system
				segmentCorner = glm::inverse(camera->getPV()) * vec4((j % 2) ? 1.0f : -1.0f, ((j / 2) % 2) ? 1.0f : -1.0f, ((j >= 4) ? currZ : zSegment), 1.0f) / (camera->getFar() - camera->getNear());
				//calc segment corner in sun coordinate system
				segmentCorner = this->getWorldTransform().getMatrix() * glm::vec4(segmentCorner, 1.0f);

				segmentMin = vec3(glm::min(segmentCorner.x, segmentMin.x), glm::min(segmentCorner.y, segmentMin.y), glm::min(segmentCorner.z, segmentMin.z));
				segmentMax = vec3(glm::max(segmentCorner.x, segmentMax.x), glm::max(segmentCorner.y, segmentMax.y), glm::max(segmentCorner.z, segmentMax.z));
			}
			Transform camTransform = this->transform;
			camTransform.setPosition(camera->getWorldTransform().getPosition() + camera->getWorldTransform().forward() * (currZ + zSegment) / 2.0f);
			currZ = zSegment;
			vec3 center = (segmentMax + segmentMin) / 2.0f;
			vec3 scale = (segmentMax - segmentMin);
			scale = vec3(glm::max(glm::max(scale.x, scale.y), scale.z));

			((Camera*)(shadowCamera[i]))->setOrthoProjection(scale.x * 2.4f, scale.y * 2.4f, -shadowFar, scale.z);
			((Camera*)(shadowCamera[i]))->setWorldTransform(camTransform);
		}
	public:
		unsigned char getCascadeDrawRequiredMask() { return cascadeCounter ^ (cascadeCounter - 1); }

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
			
			vec3 front = camera->getRelativeTransform().forward();
			vec3 segmentCorner;
			float ratioSum = 0.0f;
			float currZ = camera->getNear();
			float zSegment;
			for (unsigned int i = 0; i < cascades; i++) ratioSum += this->cascadesRatio[i];
			unsigned char drawMask = (cascadeCounter ^ (cascadeCounter + 1));
			int cascadeBit = 1 << (cascades - 1);
			for (unsigned int i = this->cascades - 1; i >= 0; i--)
			{
				if (cascadeBit & drawMask)
				{
					updateCameraPos(i, ratioSum, currZ, zSegment, segmentCorner, camera);
					break;
				}
				cascadeBit = cascadeBit >> 1;
			}
			cascadeCounter++;
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

			dLightShader.setUniform("lightModel", PV, cascades);
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
				((Camera*)shadowCamera[i])->renderTexture.setBorderColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
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