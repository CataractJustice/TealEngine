#include "Rect2D.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Mesh/MeshRenderer.h"

namespace TealEngine
{
	Rect2D::Rect2D(glm::vec2 offset, glm::vec2 scale, glm::vec4 color, const string& textureName) : offset(offset), scale(scale), color(color), textureName(textureName)
	{

	}

	void Rect2D::Draw() 
	{
		ShaderProgram& shader = Core::shadersManager.get("Rect2D");
		shader.setUniform("offset", this->offset);
		shader.setUniform("scale", this->scale);
		shader.setUniform("color", this->color);
		shader.setTexture("textureImage", 
		(textureName.length() && Core::textureManager.exists(textureName)) ? Core::textureManager.get(textureName).id() : Core::textureManager.get("bd.bmp").id()
		);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		MeshRenderer::render(BasicMeshes::square.sharedMesh(), &shader, GL_TRIANGLES);
	};
} // namespace TealEngine
