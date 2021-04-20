#pragma once
#include<iostream>
#include <glew.h>
#include <GLFW/glfw3.h>
#include "libs/glm/vec2.hpp"
#include "libs/glm/vec3.hpp"
#include "libs/glm/vec4.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

class Uniform
{
protected:
	std::string name;
	GLuint location;

public:
	virtual void use() = 0;
	virtual Uniform* clone() = 0;
	std::string getName()
	{
		return this->name;
	}
};

template <class T>
class TemplateUniform : public Uniform
{
protected:
	T* value;
	unsigned int length;
	bool shared;
	bool v;
public:
	TemplateUniform(GLuint shader, std::string name)
	{
		shared = false;
		this->name = name;
		glUseProgram(shader);
		this->location = glGetUniformLocation(shader, name.c_str());
		if (this->location == -1) std::cout << "TemplateUniform<T>::constructor(GLuint, name) ERROR: \""<<name<<"\" uniform is undefined in shader.\n";
	}

	~TemplateUniform()
	{
		if (length > 1)
			delete[] value;
		else
			delete value;
	}
	void set(T* value, unsigned int length = 1) 
	{
		if (!this->shared) delete[] this->value;
		this->value = new T[length];
		for (unsigned int i = 0; i < length; i++) this->value[i] = value[i];
				
		this->length = length;
	}
	void sharedSet(T* value, unsigned int length = 1) 
	{ 
		if (!this->value && !shared) if (this->length > 1) delete[] this->value; else delete this->value;
		this->value = value; 
		
		this->length = length; 
	}
	T get() { return this->value; }
	void use() 
	{ std::cout << "TemplateUniform<T>::use() ERROR: no implementation for T type; uniform \"" << this->getName() << "\"\n"; }

	Uniform* clone() 
	{
		if (this->shared) 
			return new TemplateUniform<T>(*this);
		else
		{
			TemplateUniform<T>* u = new TemplateUniform<T>(*this);
			if(this->value)u->set(this->value, this->length);
			return u;
		}
	}
};

template <>
void TemplateUniform<bool>::use()
{
	GLint* val = new GLint[this->length];
	for (unsigned int i = 0; i < this->length; i++) val[i] = this->value[i];
	glUniform1iv(this->location, this->length, val);
	delete[] val;
};

template <>
void TemplateUniform<float>::use()
{
	glUniform1fv(this->location, this->length, this->value);
};

template <>
void TemplateUniform<int>::use()
{
	glUniform1iv(this->location, this->length, this->value);
};

template <>
void TemplateUniform<glm::vec2>::use()
{
	glUniform2fv(this->location, this->length, glm::value_ptr(*this->value));
};

template <>
void TemplateUniform<glm::vec3>::use()
{
	glUniform3fv(this->location, this->length, glm::value_ptr(*this->value));
};

template <>
void TemplateUniform<glm::vec4>::use()
{
	glUniform4fv(this->location, this->length, glm::value_ptr(*this->value));
};

template <>
void TemplateUniform<glm::mat4>::use()
{
	glUniformMatrix4fv(this->location, this->length, GL_FALSE, glm::value_ptr(*this->value));
};