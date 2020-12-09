#pragma once
#define GLEW_STATIC
#include "ShaderProgram.h"
#include "Filesystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include <iostream>
namespace TealEngine {

	GLuint ShaderProgram::lastMaterialId = 0;
	GLuint ShaderProgram::usedMaterialId = 0;

	void ShaderProgram::loadFromFiles(std::string vertpath, std::string fragpath) 
	{
		Shader vert;
		vert.loadFromFile(vertpath, GL_VERTEX_SHADER);
		Shader frag;
		frag.loadFromFile(fragpath, GL_FRAGMENT_SHADER);
		this->link(vert.id(), frag.id());
	}

	void ShaderProgram::addTexture(std::string name)
	{
		glUseProgram(this->program);
		GLuint loc = glGetUniformLocation(this->program, name.c_str());
		if (loc == -1) std::cout << "ShaderProgram::addTexture ERROR: uniform \"" << name << "\" is undefined in shader\n";
		glUniform1i(loc, this->texture.size());
		this->texture[name] = std::pair<GLuint, GLuint>(texture.size(), 0);
	}

	void ShaderProgram::setTexture(std::string name, GLuint texture)
	{
		if (this->texture.find(name) == this->texture.end())
			addTexture(name);
		this->texture[name].second = texture;
	}

	void ShaderProgram::setTexture(std::string name, GLuint index, GLuint texture)
	{
		if (this->texture.find(name + "[" + std::to_string(index) + "]") == this->texture.end())
			addTexture(name + "[" + std::to_string(index) + "]");
		this->texture[name + "[" + std::to_string(index) + "]"].second = texture;
	}

	void ShaderProgram::link(GLuint vertexShader, GLuint fragmentShader)
	{
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader); 
		glLinkProgram(program);

		GLint success;
		GLchar* infoLog = new GLchar[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << infoLog;
		}
		lastMaterialId++;
		materialId = lastMaterialId;
	}

	void ShaderProgram::link(Shader vertexShader, Shader geometryShader, Shader fragmentShader)
	{
		program = glCreateProgram();
		glAttachShader(program, vertexShader.id());
		glAttachShader(program, fragmentShader.id());
		glAttachShader(program, geometryShader.id());
		glLinkProgram(program);
		lastMaterialId++;
		materialId = lastMaterialId;
	}

	void ShaderProgram::use()
	{
		if (usedMaterialId != materialId)
		{
			glUseProgram(program);
			usedMaterialId = materialId;

			for (auto tex : texture)
			{
				glActiveTexture(GL_TEXTURE0 + tex.second.first);
				glBindTexture(GL_TEXTURE_2D, tex.second.second);
			}
		}
		for (std::pair<std::string, std::pair<unsigned short, Uniform*>> u : this->uniform)
		{
			if (u.second.first)
			{
				u.second.second->use();
				if(u.second.first == 1) u.second.first = 0;
			}
		}
	}

	GLuint ShaderProgram::id()
	{
		return program;
	}
}