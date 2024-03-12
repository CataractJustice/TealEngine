#include "SolidBorder.h"
#include "GameNode/GameNodeGUI.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "GameNode/ComponentFactory.h"
namespace TealEngine 
{
	SolidBorder::SolidBorder() 
	{
		addProp(new ColorProp(glm::value_ptr(color)), "Color");
		//addProp(new FloatProp(&thickness), "Thickness");
	}

	void SolidBorder::GUIRender() 
	{
		GameNodeGUI* guiNode = this->getParentOfType<GameNodeGUI>();
		if(!guiNode) 
		{
			return;
		}
		GUITransform& transform = guiNode->getTransform();
		float offsetx 	= transform.getLeft().inWindowWidths(transform);
		float offsety 	= transform.getTop().inWindowHeights(transform);
		float width 	= transform.getWidth().inWindowWidths(transform);
		float height 	= transform.getHeight().inWindowHeights(transform);

		offsetx = offsetx * 2.0f - 1.0f;
		offsety = offsety * 2.0f - 1.0f;

		ShaderProgram& shader = Core::shadersManager.get("Rect2D");
		shader.setUniform("offset", glm::vec2(offsetx + width, offsety + height));
		shader.setUniform("scale", 	glm::vec2(width, height));
		shader.setUniform("color", 	color);
		shader.setTexture("textureImage", Core::textureManager.get("bd.bmp").id());
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		MeshRenderer::render(BasicMeshes::square.sharedMesh(), &shader, GL_TRIANGLES);
	}
	EXPORT_COMPONENT(SolidBorder);
}