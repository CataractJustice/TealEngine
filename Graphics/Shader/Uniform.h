#pragma once
#include<iostream>
#include "libs/glm/vec2.hpp"
#include "libs/glm/vec3.hpp"
#include "libs/glm/vec4.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

using namespace glm;

class Uniform
{
protected:
	std::string name;
	unsigned int location;
	char value[1024];
	int bytesize;
	int size;
	int type;
	void uset(void* data, int typesize, int size, int type);
public:

	void use();
	
	Uniform(unsigned int shader, std::string name, int type);

	void* valueptr() const { return (void*)value; }
	
	int getByteSize() const { return bytesize; }
	
	int getSize() const { return size; }

	virtual void set1fv(float* values, int size);
	virtual void set2fv(float* values, int size);
	virtual void set3fv(float* values, int size);
	virtual void set4fv(float* values, int size);
	virtual void setm4fv(float* values, int size);
	virtual void set1iv(int* values, int size);
	
	Uniform(const Uniform& uniform) 
	{
		if(uniform.getByteSize() > 0 && uniform.getByteSize() < 1024)
			memcpy(value, uniform.valueptr(), uniform.getByteSize());
		this->bytesize = uniform.bytesize;
		this->location = uniform.location;
		this->name = uniform.name;
		this->size = uniform.size;
		this->type = uniform.type;
	}

	Uniform& operator=(const Uniform& other) 
	{
		if (other.getByteSize() > 0 && other.getByteSize() < 1024)
			memcpy(value, other.valueptr(), other.getByteSize());
		this->bytesize = other.bytesize;
		this->location = other.location;
		this->name = other.name;
		this->size = other.size;
		this->type = other.type;
		return *this;
	}

	Uniform() : name(""), location(0), bytesize(0), size(0), type(0) {}

	std::string getName()
	{
		return this->name;
	}

	inline int getType() const { return this->type; }
};