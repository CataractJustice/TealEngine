#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderCompiler.h"

namespace Kirino 
{
	void ShaderCompiler::compile(const char* code, unsigned int type, unsigned int& shader) 
	{
		shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);
		
		int success;
		const int infoLogLength = 1024;
		char infoLog[infoLogLength];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			std::cout
			<< "\n\n\n---=== SHADER COMPILATION FAIL TYPE " << type <<" ===---\n" 
			<< infoLog << '\n'
			<< "---===SHADER SOURCE===---\n" 
			<< code 
			<< "\n---===---===---===---\n";
		}
	}
	void ShaderCompiler::link(unsigned int vert, unsigned int frag, unsigned int& program) 
	{
		program = glCreateProgram();
		glAttachShader(program, vert);
		glAttachShader(program, frag); 
		glLinkProgram(program);

		int success;
		const int infoLogLength = 1024;
		char infoLog[infoLogLength];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			std::cout 
			<< "\n\n\n---===SHADER PROGRAM LINK FAIL===---\n" 
			<< infoLog 
			<< "\n---===---===---===---\n";
		}
	}
}