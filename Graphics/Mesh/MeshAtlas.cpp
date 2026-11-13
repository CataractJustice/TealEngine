#include "MeshAtlas.h"

namespace TealEngine 
{
	MeshAtlas::MeshAtlas(int initialDataBufferSize, int initialIndiceBufferSize)
	{
		//Generate buffer
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ARRAY_BUFFER, initialDataBufferSize, nullptr, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, initialIndiceBufferSize, nullptr, GL_STATIC_DRAW);
		dataCap = initialDataBufferSize;
		indiceCap = initialIndiceBufferSize;
	};

	GLuint MeshAtlas::data(void* data, int dataSize, int* indices, int indicesSize) 
	{
		//Resizing is not yet implemented
		if(dataOffset + dataSize > dataCap) throw;
		if(indicesOffset + indicesSize > indiceCap) throw;
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ARRAY_BUFFER, dataOffset, dataSize, data);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesSize, indices);
		dataOffset += dataSize;
		indicesOffset += indicesSize;
		return indicesOffset - indicesSize;
	}
}