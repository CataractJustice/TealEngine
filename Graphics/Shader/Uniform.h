#pragma once
#include<iostream>
#include "libs/glm/vec2.hpp"
#include "libs/glm/vec3.hpp"
#include "libs/glm/vec4.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

using namespace glm;

#define DECLARE_UNIFORM(type) class type ## Uniform : public Uniform {virtual int typesize() const; void use();};

class Uniform
{
protected:
	enum UniformType 
	{
		FLOAT_UNIFORM,
		INT_UNIFORM,
		VEC2_UNIFORM,
		VEC3_UNIFORM,
		VEC4_UNIFORM,
		MAT4_UNIFORM
	};

	std::string name;
	unsigned int location;
	char* value;
	int bytesize;
	int size;
	UniformType type;
public:

	void use();
	
	Uniform(unsigned int shader, std::string name);

	const char* valueptr() { return value; }
	
	int getByteSize() { return bytesize; }
	
	int getSize() { return size; }

	virtual void set1fv(float* values, int size);
	virtual void set2fv(float* values, int size);
	virtual void set3fv(float* values, int size);
	virtual void set4fv(float* values, int size);
	virtual void setm4fv(float* values, int size);
	virtual void set1iv(int* values, int size);
	
	Uniform(Uniform& uniform) 
	{
		memcpy(value, uniform.valueptr(), uniform.getByteSize());
		this->bytesize = uniform.bytesize;
		this->location = uniform.location;
		this->name = uniform.name;
		this->size = uniform.size;
		this->value = new char[uniform.getByteSize()];
	}

	Uniform& operator=(Uniform& other) 
	{
		memcpy(value, other.valueptr(), other.getByteSize());
		this->bytesize = other.bytesize;
		this->location = other.location;
		this->name = other.name;
		this->size = other.size;
		this->value = new char[other.getByteSize()];
		return *this;
	}

	std::string getName()
	{
		return this->name;
	}
};