#pragma once
#include "RenderUtil.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Resources.h"

namespace TealEngine {
	namespace Render
	{
		using namespace Graphics;
		glm::mat4 VP_matrix;

		void renderShader(ShaderProgram* shader, glm::vec2 offset, glm::vec2 scale)
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			shader->setUniform("offset", offset);
			shader->setUniform("scale", scale);
			MeshRenderer::render(&BasicMeshes::square, shader, GL_TRIANGLES);
		}

		void rednerTexture(GLuint texture, glm::vec2 offset, glm::vec2 scale)
		{
			ShaderProgram shader = Resources::getShader("basic_2d");
			shader.setUniform("offset", offset);
			shader.setUniform("scale", scale);
			shader.setTexture("tex", texture);
			renderShader(&shader);
		}
	}
}