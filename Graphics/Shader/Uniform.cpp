#include "Uniform.h"
#include <glew.h>
#include <GLFW/glfw3.h>

#define DEFINE_UNIFORM_SET_METHOD(name, datatype, stride, enumtype)\
void Uniform:: ## name (datatype* values, int size) \
{\
	if (this->value)\
		delete[] this->value; \
	this->value = new char[sizeof(*values) * stride]; \
	memcpy(this->value, values, size * sizeof(*values)* stride); \
	this->type = enumtype; \
	this->size = size;\
}

DEFINE_UNIFORM_SET_METHOD(set1fv, float, 1, FLOAT_UNIFORM);
DEFINE_UNIFORM_SET_METHOD(set2fv, float, 2, VEC2_UNIFORM);
DEFINE_UNIFORM_SET_METHOD(set3fv, float, 3, VEC3_UNIFORM);
DEFINE_UNIFORM_SET_METHOD(set4fv, float, 4, VEC4_UNIFORM);
DEFINE_UNIFORM_SET_METHOD(setm4fv, float, 16, MAT4_UNIFORM);
DEFINE_UNIFORM_SET_METHOD(set1iv, int, 1, INT_UNIFORM);


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