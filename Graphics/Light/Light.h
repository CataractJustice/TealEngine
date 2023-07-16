#pragma once
#include "GameNode/GameNode.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Resources.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Shader/ShaderProgram.h"
#include <limits>

namespace TealEngine 
{
	class DeferredRenderer;
	class ForwardRenderer;

	extern ShaderProgram dLightShader;
	extern ShaderProgram pLightShader;
	extern ShaderProgram aLightShader;
	extern ShaderProgram combineLightShader;
	extern ShaderProgram shadowMapShader;
	extern ForwardRenderer shadowMapRenderer;
	extern void lightInit();

	class DirectionLight : public Component 
	{
		private:
		static FrameBuffer fb;
		GLuint resolution;
		GLfloat shadowFar;
		vector<GameNode*> shadowCamera;
		GLuint cascades;
		float cascadesRatio[8];

		public:
		glm::vec4 color;
		void updateCameraPos(int i, Camera* camera);

		void setCascadeRatio(unsigned int cascade, float ratio);
		Camera* getShadowCamera(int index);
		void setupUniforms(GLuint render, GLuint albedo, GLuint position, GLuint normal, GLuint specular, vec3 viewPos);

		GLuint shadowResolution();

		GLuint shadowCascades();
		DirectionLight(GLuint shadow_resolution = 1000, GLuint cascades = 3U, GLfloat shadowFar = 250.0f);
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);
		void onPropSet(const std::string& name) override;
	};

	class PointLight : public Component 
	{
		public:
		glm::vec4 color;
		glm::vec3 attenuation;
		float radius;
		PointLight(glm::vec4 color = glm::vec4(1.0f), float radius = 10.0f);
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer) override;
	};
}