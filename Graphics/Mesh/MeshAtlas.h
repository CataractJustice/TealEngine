#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace TealEngine 
{
	class MeshAtlas 
	{
		private:
		GLuint VBO, EBO;
		int dataOffset, indicesOffset, dataSize, indicesSize;
		int dataCap, indiceCap;
		public:
		MeshAtlas(int dataBufferSize, int indiceBufferSize);
		~MeshAtlas();
		//adds data to VBO and EBO with glBufferSubData and returns indices offset in bytes
		GLuint data(void* data, int dataSize, int* indices, int indicesSize);
		inline GLuint getEBO() const { return EBO; }
		inline GLuint getVBO() const { return VBO; }
	};
}