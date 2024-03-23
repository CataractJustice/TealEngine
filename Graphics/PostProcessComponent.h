#pragma once
#include "GameNode/Component.h"
#include "Renderer/RenderUtil.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
namespace TealEngine 
{
	class PostProcessComponent : public Component
	{
	private:
		ShaderProgram* material;
		bool outputUnlit, outputLit, outputPosition, outputNormal, outputSpecular, outputLight;
		FrameBuffer fb;
	public:
		PostProcessComponent();
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer) override;
	};
}
