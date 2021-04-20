#pragma once
#include "Graphics/Shader/ShaderProgram.h"
namespace TealEngine {
	namespace Render
	{
		extern glm::mat4 VP_matrix;
		void renderShader(ShaderProgram* shader);
		void rednerTexture(GLuint texture, glm::vec2 offset = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
	}
}