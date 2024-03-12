#include "Line3D.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Mesh/MeshRenderer.h"

namespace TealEngine
{
	Line3D::Line3D(glm::vec3 from, glm::vec3 to, glm::vec4 color, float thickness) : from(from), to(to), color(color), thickness(thickness)
	{

	}

	void Line3D::Draw() 
	{
		ShaderProgram& shader = Core::shadersManager.get("Line3D");
		shader.setUniform("from", this->from);
		shader.setUniform("to", this->to);
		shader.setUniform("color", this->color);
		shader.setUniform("thickness", this->thickness);
		shader.setUniform("pv_mat", Core::renderer.getActiveCamera()->getPV());
		shader.setUniform("model", glm::mat4(1.0f));
		shader.setTexture("textureImage", Core::textureManager.get("bd.bmp").id());
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		MeshRenderer::render(BasicMeshes::square.sharedMesh(), &shader, GL_TRIANGLES);
	};
} // namespace TealEngine
