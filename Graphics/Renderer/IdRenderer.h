#pragma once
#include "ForwardRenderer.h"

namespace TealEngine 
{
	class IdRenderer : public ForwardRenderer
	{
		public:
		static void loadIdShader();
		void render(GameNode* scene);
		static ShaderProgram idShader;
	};
}