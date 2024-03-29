#include "MeshRenderer.h"
#include <GL/glew.h>
#include "GameNode/ComponentFactory.h"
#include "Core.h"
#include "GameNode/ComponentProp.h"
#include "Graphics/Renderer/IdRenderer.h"


namespace TealEngine 
{
	MeshRenderer::MeshRenderer()
	{
		addProp(new StringKeyMapValuePointerProp<SharedMesh>((SharedMesh**)(&(this->mesh)), &Core::modelsManager.getLoadedMap()), "Mesh");
		addProp(new StringKeyMapValuePointerProp<ShaderProgram>((ShaderProgram**)(&(this->shader)), &Core::materialsManager.getLoadedMap()), "Material");
		addProp(new BoolProp(&this->shadowCaster), "Shadow caster");
		this->shadowCaster = true;
		mesh = nullptr;
		shader = nullptr;
		this->mode = GL_TRIANGLES;
		this->renderPassMask = (unsigned int)RenderPass::DeferredPass | (unsigned int)RenderPass::ShadowMapPass;
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

	bool MeshRenderer::isShadowCaster() 
	{
		return this->shadowCaster;
	}

	void MeshRenderer::setShadowCast(bool castShadow) 
	{
		this->shadowCaster = castShadow;
		if(castShadow)
			this->renderPassMask |=	((unsigned int)(RenderPass::ShadowMapPass));
		else
			this->renderPassMask &=	~((unsigned int)(RenderPass::ShadowMapPass));
	}
	
	EXPORT_COMPONENT(MeshRenderer);
	EXPORT_COMPONENT(Mesh3DRenderer);
}