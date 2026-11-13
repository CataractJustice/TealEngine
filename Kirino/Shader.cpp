#include <GL/glew.h>
#include "Shader.h"
#include "ShaderCompiler.h"
namespace Kirino
{
	Shader::Shader() : id(0)
	{
	}

	Shader::Shader(const char* source, unsigned int type) : id(0)
	{
		this->compile(source, type);
	}

	Shader::~Shader() 
	{
		if(id) 
		{
			//delete shader
			glDeleteShader(id);
		}
	}

	void Shader::compile(const char* source, unsigned int type) 
	{
		if(id) 
		{
			throw "Shader compiled twice, shouldn't happen.";
		}
		ShaderCompiler::compile(source, type, id);
	}
} // namespace Kirino
