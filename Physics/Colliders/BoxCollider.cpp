#include "BoxCollider.h"
#include "Core.h"
#include "Graphics/Renderer/Shapes/Line3D.h"
#include "GameNode/GameNode3D.h"
#include "Graphics/Mesh/MeshRenderer.h"

namespace TealEngine
{		
	BoxCollider::BoxCollider() 
	{
		this->from = glm::vec3(-1.0f);
		this->to = glm::vec3(1.0f);
		this->addProp(new FloatVecProp(glm::value_ptr(this->from), 3), "From");
		this->addProp(new FloatVecProp(glm::value_ptr(this->to), 3), "To");
	}
	
	void BoxCollider::render(ShaderProgram* shader, unsigned int stages)
	{
		if(GameNode::getSelectedNode() != this->getParent()) return;
		if(!(((unsigned int)MeshRenderer::RenderPass::DebugPass) & stages)) return; 
		Transform& transform = this->getParentOfType<GameNode3D>()->getWorldTransform();

		glm::vec3 c000 = transform.pointFromLocalToGlobal(glm::vec3(from.x, from.y, from.z));
		glm::vec3 c001 = transform.pointFromLocalToGlobal(glm::vec3(from.x, from.y, to.z));
		glm::vec3 c010 = transform.pointFromLocalToGlobal(glm::vec3(from.x, to.y, from.z));
		glm::vec3 c011 = transform.pointFromLocalToGlobal(glm::vec3(from.x, to.y, to.z));
		glm::vec3 c100 = transform.pointFromLocalToGlobal(glm::vec3(to.x, from.y, from.z));
		glm::vec3 c101 = transform.pointFromLocalToGlobal(glm::vec3(to.x, from.y, to.z));
		glm::vec3 c110 = transform.pointFromLocalToGlobal(glm::vec3(to.x, to.y, from.z));
		glm::vec3 c111 = transform.pointFromLocalToGlobal(glm::vec3(to.x, to.y, to.z));
		glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float thickness = 0.01f;

		Core::shapesRenderer.pushShape(Line3D(c000, c001, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c000, c010, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c000, c100, color, thickness));

		Core::shapesRenderer.pushShape(Line3D(c111, c110, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c111, c101, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c111, c011, color, thickness));

		Core::shapesRenderer.pushShape(Line3D(c101, c001, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c101, c100, color, thickness));

		Core::shapesRenderer.pushShape(Line3D(c011, c001, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c011, c010, color, thickness));

		Core::shapesRenderer.pushShape(Line3D(c110, c010, color, thickness));
		Core::shapesRenderer.pushShape(Line3D(c110, c100, color, thickness));
	}
} 
// namespace TealEngine