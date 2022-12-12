#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "Shader.h"
#include "FileSystem/loadStrFromFile.h"
namespace TealEngine
{
	void Shader::loadFromFile(std::string path, unsigned int type)
	{
		loadFromString(loadStrFromFile(path), type);
	}

	void Shader::loadFromString(std::string source, unsigned int type)
	{
		shader = glCreateShader(type);

		const GLchar* src = source.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		GLint success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	GLuint Shader::id()
	{
		return shader;
	}

	Shader::~Shader()
	{
		glDeleteShader(shader);
	}
}