#pragma once
#include "Graphics/Shader/ShaderProgram.h"
namespace TealEngine {
	namespace Render
	{
		extern glm::mat4 VP_matrix;
		extern glm::mat4 V_matrix;
		void renderShader(ShaderProgram* shader, glm::vec2 offset = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
		void renderTexture(unsigned int texture, glm::vec2 offset = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
	}
}