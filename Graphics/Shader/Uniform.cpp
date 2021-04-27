#include "Uniform.h"
#include <glew.h>
#include <GLFW/glfw3.h>

#define DEFINE_UNIFORM_SET_METHOD(name, datatype, stride, enumtype)\
void Uniform:: ## name (datatype* values, int size) \
{\
	uset((void*)values, sizeof(datatype) * stride, size, enumtype);\
}

void Uniform::uset(void* values, int typesize, int size, Uniform::UniformType type) 
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
	uset(values, sizeof(float), size, FLOAT_UNIFORM);
}
void Uniform::set2fv(float* values, int size)
{
	uset(values, sizeof(float) * 2, size, VEC2_UNIFORM);
}
void Uniform::set3fv(float* values, int size)
{
	uset(values, sizeof(float) * 3, size, VEC3_UNIFORM);
}
void Uniform::set4fv(float* values, int size)
{
	uset(values, sizeof(float) * 4, size, VEC4_UNIFORM);
}
void Uniform::setm4fv(float* values, int size)
{
	uset(values, sizeof(float) * 16, size, MAT4_UNIFORM);
}
void Uniform::set1iv(int* values, int size)
{
	uset(values, sizeof(int), size, INT_UNIFORM);
}



void Uniform::use() 
{
	switch (type)
	{
	case Uniform::FLOAT_UNIFORM:
		glUniform1fv(location, size, (float*)value);
		break;
	case Uniform::INT_UNIFORM:
		glUniform1iv(location, size, (int*)value);
		break;
	case Uniform::VEC2_UNIFORM:
		glUniform2fv(location, size, (float*)value);
		break;
	case Uniform::VEC3_UNIFORM:
		glUniform3fv(location, size, (float*)value);
		break;
	case Uniform::VEC4_UNIFORM:
		glUniform4fv(location, size, (float*)value);
		break;
	case Uniform::MAT4_UNIFORM:
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