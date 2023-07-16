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
		static const int OutputLayout = 0;
		static const int AlbedoLayout = 1;
		static const int PositionLayout = 2;
		static const int NormalLayout = 3;
		static const int SpecularLayout = 4;
		static const int LightLayout = 5;
	};
}