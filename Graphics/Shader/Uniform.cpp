#include "Uniform.h"
#include <glew.h>
#include <GLFW/glfw3.h>

void Uniform::uset(void* values, int typesize, int size, int type) 
{
	if (this->value)
		delete[] this->value;
	this->value = new char[typesize * size];
	memcpy(this->value, values, typesize * size);
	this->type = type;
	this->size = size;
	this->bytesize = typesize * size;
}

void Uniform::set1fv(float* values, int size) 
{
	uset(values, sizeof(float), size, GL_FLOAT);
}
void Uniform::set2fv(float* values, int size)
{
	uset(values, sizeof(float) * 2, size, GL_FLOAT_VEC2);
}
void Uniform::set3fv(float* values, int size)
{
	uset(values, sizeof(float) * 3, size, GL_FLOAT_VEC3);
}
void Uniform::set4fv(float* values, int size)
{
	uset(values, sizeof(float) * 4, size, GL_FLOAT_VEC4);
}
void Uniform::setm4fv(float* values, int size)
{
	uset(values, sizeof(float) * 16, size, GL_FLOAT_MAT4);
}
void Uniform::set1iv(int* values, int size)
{
	uset(values, sizeof(int), size, GL_INT);
}



void Uniform::use() 
{
	switch (type)
	{
	case GL_FLOAT:
		glUniform1fv(location, size, (float*)value);
		break;
	case GL_INT:
		glUniform1iv(location, size, (int*)value);
		break;
	case GL_FLOAT_VEC2:
		glUniform2fv(location, size, (float*)value);
		break;
	case GL_FLOAT_VEC3:
		glUniform3fv(location, size, (float*)value);
		break;
	case GL_FLOAT_VEC4:
		glUniform4fv(location, size, (float*)value);
		break;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(location, size, false, (float*)value);
		break;
	default:
		break;
	}
}

Uniform::Uniform(unsigned int shader, std::string name)
{
	this->name = name;
	glUseProgram(shader);
	this->location = glGetUniformLocation(shader, name.c_str());
	if (this->location == -1) std::cout << "TemplateUniform<T>::constructor(GLuint, name) ERROR: \"" << name << "\" uniform is undefined in shader.\n";
}