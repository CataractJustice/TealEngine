#pragma once
#include "GameNode/GameNode.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "Graphics/Texture.h"
#include "Kirino/ShaderProgram.h"
#include "Kirino/Material.h"
#include "Kirino/UniformBufferObject.h"
#include "Graphics/Texture.h"
#include <limits>

namespace TealEngine 
{
	class DeferredRenderer;
	
	extern void lightInit();

	class ILight : public Component 
	{
		public:
		virtual void renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) = 0;
	};

	class DirectionLight : public ILight 
	{
		private:
		std::vector<GameNode*> shadowCamera;
		Texture shadowMaps[8];
		FrameBuffer frameBuffers[8];
		FrameBuffer depthPostFramebuffers[8];
		unsigned int cascades;
		unsigned int resolution;
		float cascadesRatio[8];
		float shadowFar;
		glm::vec4 color;
		glm::vec4 indirectColor;
		unsigned int frameCounter;
		public:
		void updateCameraPos(int i, Camera* camera);

		void setCascadeRatio(unsigned int cascade, float ratio);
		Camera* getShadowCamera(int index);
		void setupUniforms(const RenderTexturesCollection& renderTextures, glm::vec3 viewPos);

		GLuint shadowResolution();

		GLuint shadowCascades();
		DirectionLight(GLuint shadow_resolution = 1000, GLuint cascades = 3U, GLfloat shadowFar = 250.0f);
		~DirectionLight();
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);
		void onPropSet(const std::string& name) override;

		void renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) override;
	};

	class PointLight : public ILight 
	{
		public:
		glm::vec4 color;
		glm::vec3 position;
		glm::vec3 CLQRatios;
		PointLight(glm::vec4 color = glm::vec4(1.0f), float radius = 10.0f);
		void renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) override;
	};

	class SSAO : public ILight 
	{
		private:
		float strength;
		float radius;
		glm::vec4 ambient;
		Texture ssaoBufferTexture;
		FrameBuffer fbo;
		public:
		SSAO();	
		void renderLight(const RenderTexturesCollection& renderTextures, Camera* camera, DeferredRenderer* renderer) override;
	};
}