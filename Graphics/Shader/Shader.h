#pragma once
#include <glew.h>
#include <string>
#include <vector>
#include "Uniform.h"
namespace TealEngine
{
	class Shader
	{
	private:
		GLuint shader;
		std::vector<Uniform*> uniforms;
	public:
		std::vector<Uniform*> getUniforms() 
		{
		
		}
		void loadFromFile(std::string path, GLenum type);

		void loadFromString(std::string source, GLenum type);

		GLuint id();

		~Shader();
	};
}

