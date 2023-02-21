#include "MeshRenderer.h"
#include <GL/glew.h>
#include "GameNode/ComponentFactory.h"
#include "Resources.h"
#include "GameNode/ComponentProp.h"
#include "Graphics/Renderer/IdRenderer.h"


namespace TealEngine 
{
	MeshRenderer::MeshRenderer()
	{
		addProp(new StringKeyMapValuePointerProp<Mesh>((Mesh**)(&(this->mesh)), &Resources::getModelsMap()), "Mesh");
		addProp(new StringKeyMapValuePointerProp<ShaderProgram>((ShaderProgram**)(&(this->shader)), &Resources::getMaterialsMap()), "Material");
		mesh = nullptr;
		shader = nullptr;
		this->mode = GL_TRIANGLES;
		this->activeRenderStages = (unsigned int)RenderStage::Lit;
	}
	
	void Mesh3DRenderer::renderId()
	{
		if(!this->mesh) return;
		if (mesh->getLength() > 0) 
		{	
			Transform meshTransform = ((GameNode3D*)getParent())->getWorldTransform();
			
			vec4 screenPos = Render::VP_matrix * vec4(meshTransform.getPosition(), 1.0f);
			//to-do: culling
			if (true) 
			{
				ShaderProgram* targetShader = &IdRenderer::idShader;

				targetShader->setUniform("model", meshTransform.getMatrix());
				targetShader->setUniform("pv_mat", Render::VP_matrix);
				targetShader->setUniform("id", getParent()->getId());
				MeshRenderer::render(targetShader, mode ? mode : this->mode, 0);
			}
		}
	}
	
	EXPORT_COMPONENT(MeshRenderer);
	EXPORT_COMPONENT(Mesh3DRenderer);
}