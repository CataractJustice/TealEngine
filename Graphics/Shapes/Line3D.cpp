#include "Line3D.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Kirino/Material.h"

namespace TealEngine
{
	Line3D::Line3D(glm::vec3 from, glm::vec3 to, glm::vec4 color, float thickness) : from(from), to(to), color(color), thickness(thickness)
	{
	}

	void Line3D::Draw() 
	{
		Kirino::Material material;
		material.setProgram(&Core::shadersManager.get("Line3D"));
		material.setUniform("from", &from, sizeof(from), 0);
		material.setUniform("to", &to, sizeof(to), 0);
		material.setUniform("color", &color, sizeof(color), 0);
		material.setUniform("thickness", &thickness, sizeof(thickness), 0);
		material.setUniformBlock("CameraBlock", Core::cameraUBO.getId());
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		Render::renderShader(&material);
	};
} // namespace TealEngine