#include "SelectedObjectOutlinePostProcess.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
namespace TealEngine 
{
	void SelectedObjectOutlinePostProcess::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light) 
	{
		if(GameNode::getSelectedNode() == nullptr) return;
		SelectedObjectOutlinePostProcess::outlineShader.setTexture("Unlit", unlitColor);
		SelectedObjectOutlinePostProcess::outlineShader.setTexture("IdMap", Core::idRenderer.getActiveCamera()->renderTexture.id());
		SelectedObjectOutlinePostProcess::outlineShader.setUniform("id", GameNode::getSelectedNode()->getId());
		SelectedObjectOutlinePostProcess::outlineShader.setUniform("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Render::renderShader(&SelectedObjectOutlinePostProcess::outlineShader);
	}
	void SelectedObjectOutlinePostProcess::loadShader() 
	{
		std::map<std::string, ShaderProgram> outlineShaderMap;
		ShaderProgram::loadShadersFromJson(outlineShaderMap, "./Assets/Shaders/SelectedOutlineShader.json");
		SelectedObjectOutlinePostProcess::outlineShader = outlineShaderMap["SelectedOutlineShader"];
	}

	ShaderProgram SelectedObjectOutlinePostProcess::outlineShader;
}