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
	class DefferedRenderer;
	class ForwardRenderer;

	extern ShaderProgram dLightShader;
	extern ShaderProgram aLightShader;
	extern ShaderProgram combineLightShader;
	extern ShaderProgram shadowMapShader;
	extern void lightInit();

	class Light : public Component
	{
	private:
		static std::vector<Light*> lights;
	protected:
		static ForwardRenderer shadowMapRenderer;
		static FrameBuffer fb;

	public:
		vec4 color;
		Light();
		virtual void render(Texture& light, Texture& albedo, Texture& position, Texture& normal, Texture& specular, DefferedRenderer* renderer, GameNode* scene) = 0;
		static void renderLightMap(Texture& light, Texture& albedo, Texture& position, Texture& normal, Texture& specular, DefferedRenderer* renderer, GameNode* scene);
	};

	class DirectionLight : public Light 
	{
	public:

	private:
		GLuint resolution;
		GLfloat shadowFar;
		vector<GameNode*> shadowCamera;
		GLuint cascades;
		float cascadesRatio[8];
		
		void updateCameraPos(int i, Camera* camera);
	public:

		void setCascadeRatio(unsigned int cascade, float ratio);

		Camera* getShadowCamera(int index);

		void setupUniforms(GLuint render, GLuint albedo, GLuint position, GLuint normal, GLuint specular, vec3 viewPos);

		GLuint shadowResolution();

		GLuint shadowCascades();
		DirectionLight(GLuint shadow_resolution, GLuint cascades = 3U, GLfloat shadowFar = 250.0f);
		virtual void render(Texture& light, Texture& albedo, Texture& position, Texture& normal, Texture& specular, DefferedRenderer* renderer, GameNode* scene);
	};
}