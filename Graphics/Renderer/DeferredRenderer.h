#pragma once
#include "ForwardRenderer.h"
#include "Graphics/Light/Light.h"
namespace TealEngine {
	class DeferredRenderer : public ForwardRenderer
	{
	private:
		Texture albedo, position, normal, specular, light;
		ShaderProgram empty3D;
	public:
		void resize(GLuint width, GLuint height);
		void render(GameNode* scene);
	};
}