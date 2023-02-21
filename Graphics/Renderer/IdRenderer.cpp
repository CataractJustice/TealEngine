#include "IdRenderer.h"
#include "Core.h"

namespace TealEngine 
{
	
	void IdRenderer::loadIdShader()
	{
		std::map<std::string, ShaderProgram> idShaderMap;
		ShaderProgram::loadShadersFromJson(idShaderMap, "./Assets/Shaders/IdShader.json");
		IdRenderer::idShader = idShaderMap["IdShader"];
	}

	void IdRenderer::render(GameNode* scene)
	{
		//resize(activeCamera->renderTexture.getWidth(), activeCamera->renderTexture.getHeight());
		fb.attachTexture(activeCamera->renderTexture.id(), 0);
		fb.bind();
		fb.apply();
		applyConfig();
		Render::VP_matrix = Core::renderer.getActiveCamera()->getPV();
		Render::V_matrix = Core::renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform().getMatrix();
		scene->renderId();
	}

	ShaderProgram IdRenderer::idShader;
}
