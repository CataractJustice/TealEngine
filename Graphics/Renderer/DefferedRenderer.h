#pragma once
#include "ForwardRenderer.h"
#include "Graphics/Light/Light.h"
namespace TealEngine {
	class DefferedRenderer : public ForwardRenderer
	{
	private:
		Texture albedo, position, normal, specular, light;
		list<Light*> lights;
		ForwardRenderer shadowMapRenderer;
		ShaderProgram empty3D;
		void renderLights();
	public:
		void resize(GLuint width, GLuint height);
		void render();
		virtual void pushLight(Light* light);
		virtual void popLight(Light* light);
		virtual void push(MeshRenderer* mesh);
		virtual void push(GameNode* scene);

	};
}