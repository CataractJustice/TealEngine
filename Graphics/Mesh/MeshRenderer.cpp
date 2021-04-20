#include "MeshRenderer.h"
#include <glew.h>
namespace TealEngine 
{
	MeshRenderer::MeshRenderer()
	{
		this->addTag("MeshRenderer");
		this->mode = GL_TRIANGLES;
	}
}