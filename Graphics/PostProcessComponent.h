/*#pragma once
#include "GameNode/Component.h"
#include "Renderer/RenderUtil.h"
namespace TealEngine 
{
	class PostProcessComponent : public Component
	{
	private:
		ShaderProgram* material;
	public:
		PostProcessComponent(ShaderProgram* material) : material(material) {};
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light) override 
		{
			if(material) 
			{
				Render::renderShader(material);
			}
		}
	};
}*/