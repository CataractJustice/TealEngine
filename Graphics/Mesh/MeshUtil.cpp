#define GLEW_STATIC
#include "Kirino/Mesh.h"
#include "MeshUtil.h"
#include <GL/glew.h>
namespace TealEngine
{
	namespace BasicMeshes 
	{
		Kirino::Mesh screenSquare;
		Kirino::VertexArrayObject screenSquareVao;
		unsigned int screenSquareVBO;
		unsigned int screenSquareEBO;
		float screenSquareVertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};
		unsigned int screenSquareIndices[] = 
		{
			0, 1, 2,
			2, 3, 0
		};
		void init()
		{
			screenSquareVao.create();
			screenSquare.vao = &screenSquareVao;
			screenSquare.indexCount = 6;
			screenSquare.indexOffset = 0;
			screenSquare.vertexOffset = 0;
			glGenBuffers(1, &screenSquareVBO);
			glGenBuffers(1, &screenSquareEBO);
			glBindBuffer(GL_ARRAY_BUFFER, screenSquareVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(screenSquareVertices), screenSquareVertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenSquareEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenSquareIndices), screenSquareIndices, GL_STATIC_DRAW);
			screenSquare.vao->bindElementArray(screenSquareEBO);
			screenSquare.vao->vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, screenSquareVBO, 0, 2, GL_FLOAT, false, sizeof(float) * 4, 0, 0));
			screenSquare.vao->vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, screenSquareVBO, 1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2), 0));
			screenSquare.vao->updateAttribs();
		}
	}
}