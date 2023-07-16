#include "RenderUtil.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Core.h"

namespace TealEngine {
	namespace Render
	{
		using namespace Graphics;
		glm::mat4 VP_matrix;
		glm::mat4 V_matrix;

		void renderShader(ShaderProgram* shader, glm::vec2 offset, glm::vec2 scale)
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_CULL_FACE);
			shader->setUniform("offset", offset);
			shader->setUniform("scale", scale);
			MeshRenderer::render(&BasicMeshes::square, shader, GL_TRIANGLES);
		}

		void renderTexture(unsigned int texture, glm::vec2 offset, glm::vec2 scale)
		{
			ShaderProgram& shader = Core::shadersManager.get("Image");
			shader.setTexture("textureImage", texture);
			renderShader(&shader, offset, scale);
		}
	}
}