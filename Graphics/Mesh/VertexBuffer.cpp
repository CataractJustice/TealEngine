#include "VertexBuffer.h"
#include <memory.h>
namespace TealEngine 
{
	bool VertexBuffer::isVertexAttributeEnabled(GLuint layoutIndex) const
	{
		for(const VertexAttribute& vertexAttribute : vertexAttributes) 
		{
			if(vertexAttribute.index == layoutIndex) 
			{
				return vertexAttribute.enabled;
			}
		}
		return false;
		}

	VertexAttribute& VertexBuffer::addVertexAttribute(GLuint layoutIndex, GLuint size, GLenum type) 
	{
		unsigned int bytesize = sizeof(float) * size;
		for(unsigned int i = 0; i < vertexAttributes.size(); i++) 
		{
			if(vertexAttributes[i].type == 0) 
			{
				vertexAttributes[i] = {layoutIndex, size, type, true, bytesize, i, nullptr, 0};
				return vertexAttributes[i];
			}
		}
		unsigned int id = vertexAttributes.size();
		this->vertexAttributes.push_back({layoutIndex, size, type, true, bytesize, id, nullptr, 0});
		return vertexAttributes.back();
	}
	
	void VertexBuffer::removeVertexAttribute(const VertexAttribute& vertexAttribute) 
	{
		vertexAttributes[vertexAttribute.id].index = 0;
		vertexAttributes[vertexAttribute.id].size = 0;
		vertexAttributes[vertexAttribute.id].type = 0;
		vertexAttributes[vertexAttribute.id].enabled = 0;
		vertexAttributes[vertexAttribute.id].bytesize = 0;
		vertexAttributes[vertexAttribute.id].id = 0;
		vertexAttributes[vertexAttribute.id].data = 0;
		vertexAttributes[vertexAttribute.id].vertices = 0;
	}

	void VertexBuffer::build() 
	{
		unsigned int vertices = 0;
		unsigned int vertexSize = 0;
		unsigned int vertexOffset = 0;

		for(const VertexAttribute& vertexAttribute : vertexAttributes) 
		{
			vertices = std::max(vertexAttribute.vertices, vertices); 
			vertexSize += vertexAttribute.bytesize;
		}

		if(bufferSize < vertices * vertexSize) 
		{
			if(buffer) delete[] buffer;
			bufferSize = vertices * vertexSize;
			buffer = new char[bufferSize];
		}

		if(!bufferSize) return;

		memset(buffer, 0, bufferSize);

		for(const VertexAttribute& vertexAttribute : vertexAttributes) 
		{
			for(unsigned int i = 0; i < vertexAttribute.vertices; i++) 
			{
				if(vertexAttribute.bytesize) 
				{
					memcpy(buffer + i * vertexSize + vertexOffset, ((char*)vertexAttribute.data) + i * vertexAttribute.bytesize, vertexAttribute.bytesize);
				}
			}
			vertexOffset += vertexAttribute.bytesize;
		}

		if(!VBO) 
		{
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);
		}
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, bufferUsage);


		vertexOffset = 0;

		for(const VertexAttribute& vertexAttribute : vertexAttributes) 
		{
			if(vertexAttribute.type == 0) continue;
			glVertexAttribPointer(vertexAttribute.index, vertexAttribute.size, vertexAttribute.type, GL_FALSE, vertexSize, (GLvoid*)((long)vertexOffset));		
			if(vertexAttribute.enabled) glEnableVertexAttribArray(vertexAttribute.index);
			vertexOffset += vertexAttribute.bytesize;
		}
	}

	void VertexBuffer::enableVertexAttribute(const VertexAttribute& vertexAttribute) 
	{
		vertexAttributes[vertexAttribute.id].enabled = true;
		glEnableVertexAttribArray(vertexAttributes[vertexAttribute.id].index);
	}
	void VertexBuffer::disableVertexAttribute(const VertexAttribute& vertexAttribute) 
	{
		vertexAttributes[vertexAttribute.id].enabled = false;
		glDisableVertexAttribArray(vertexAttributes[vertexAttribute.id].index);
	}

	VertexBuffer::VertexBuffer() 
	{
		this->buffer = nullptr;
		this->bufferSize = 0;
		this->VAO = 0;
		this->VBO = 0;
		this->vertexAttributes = std::vector<VertexAttribute>(0);
	}

	VertexBuffer::~VertexBuffer() 
	{
		if(buffer) delete[] buffer;
	}
}