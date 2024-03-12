#include "PostProcessComponent.h"
#include "GameNode/ComponentFactory.h"
#include "Core.h"

namespace TealEngine 
{
	PostProcessComponent::PostProcessComponent()
	{
		addProp(new StringKeyMapValuePointerProp<ShaderProgram>((ShaderProgram**)(&(this->material)), &Core::materialsManager.getLoadedMap()), "Material");
		addProp(new BoolProp(&this->outputUnlit	), "Output unlit color");
		addProp(new BoolProp(&this->outputLit), "Output lit color");
		addProp(new BoolProp(&this->outputPosition), "Output position");
		addProp(new BoolProp(&this->outputNormal), "Output normal");
		addProp(new BoolProp(&this->outputSpecular), "Output specular");
		addProp(new BoolProp(&this->outputLight), "Output light");
		outputUnlit = outputPosition = outputNormal = outputSpecular = outputLight = false;
		material = nullptr;
	}
	void PostProcessComponent::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer) 
	{
		if(material) 
		{
			material->setTexture("UnlitColorMap", unlitColor);
			material->setTexture("LitColorMap", litColor);
			material->setTexture("PositionMap", position);
			material->setTexture("NormalMap", normal);
			material->setTexture("SpecularMap", specular);
			material->setTexture("LightMap", light);
			if(Core::renderer.getActiveCamera()) 
			{
				material->setUniform("CamPos", Core::renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform().getPosition());
			}
			if(outputUnlit) 
				frameBuffer->enable(DeferredRenderer::OutputLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::OutputLayout);
			if(outputLit) 
				frameBuffer->enable(DeferredRenderer::AlbedoLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::AlbedoLayout);
			if(outputPosition) 
				frameBuffer->enable(DeferredRenderer::PositionLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::PositionLayout);
			if(outputNormal) 
				frameBuffer->enable(DeferredRenderer::NormalLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::NormalLayout);
			if(outputSpecular) 
				frameBuffer->enable(DeferredRenderer::SpecularLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::SpecularLayout);
			if(outputLight) 
				frameBuffer->enable(DeferredRenderer::LightLayout); 
			else 
				frameBuffer->disable(DeferredRenderer::LightLayout);
			frameBuffer->apply();
			glBlendFunc(GL_ONE, GL_ZERO);
			Render::renderShader(material);
		}
	}
	EXPORT_COMPONENT(PostProcessComponent);
}
