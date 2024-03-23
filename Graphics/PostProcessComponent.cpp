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
			fb.bind();
			fb.attachTexture(unlitColor, DeferredRenderer::OutputLayout);
			fb.attachTexture(litColor, DeferredRenderer::AlbedoLayout);
			fb.attachTexture(position, DeferredRenderer::PositionLayout);
			fb.attachTexture(normal, DeferredRenderer::NormalLayout);
			fb.attachTexture(specular, DeferredRenderer::SpecularLayout);
			fb.attachTexture(light, DeferredRenderer::LightLayout);
			if(Core::renderer.getActiveCamera()) 
			{
				material->setUniform("CamPos", Core::renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform().getPosition());
			}
			if(outputUnlit) 
				fb.enable(DeferredRenderer::OutputLayout); 
			else 
				fb.disable(DeferredRenderer::OutputLayout);
			if(outputLit) 
				fb.enable(DeferredRenderer::AlbedoLayout); 
			else 
				fb.disable(DeferredRenderer::AlbedoLayout);
			if(outputPosition) 
				fb.enable(DeferredRenderer::PositionLayout); 
			else 
				fb.disable(DeferredRenderer::PositionLayout);
			if(outputNormal) 
				fb.enable(DeferredRenderer::NormalLayout); 
			else 
				fb.disable(DeferredRenderer::NormalLayout);
			if(outputSpecular) 
				fb.enable(DeferredRenderer::SpecularLayout); 
			else 
				fb.disable(DeferredRenderer::SpecularLayout);
			if(outputLight) 
				fb.enable(DeferredRenderer::LightLayout); 
			else 
				fb.disable(DeferredRenderer::LightLayout);
			fb.apply();
			glBlendFunc(GL_ONE, GL_ZERO);
			Render::renderShader(material);
			fb.unbind();
		}
	}
	EXPORT_COMPONENT(PostProcessComponent);
}
