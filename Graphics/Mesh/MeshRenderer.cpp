#include "MeshRenderer.h"
#include <GL/glew.h>
namespace TealEngine 
{
	MeshRenderer::MeshRenderer()
	{
		this->mode = GL_TRIANGLES;
		this->activeRenderStages = (unsigned int)RenderStage::Lit;
	}
}