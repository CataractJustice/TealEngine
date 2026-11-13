#pragma once
#include "Kirino/Material.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/matrix.hpp"
namespace TealEngine {
	namespace Render
	{
		extern glm::mat4 VP_matrix;
		extern glm::mat4 V_matrix;
		void renderShader(Kirino::Material* shader);
		void renderTexture(unsigned int texture);
	}
}