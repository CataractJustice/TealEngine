#include "UniformBufferObject.h"

namespace Kirino
{
	UniformBufferObject::UniformBufferObject() 
	{
		id = 0;
	}
	UniformBufferObject::~UniformBufferObject() 
	{
		if(id)
			glDeleteBuffers(1, &id);
	}
	void UniformBufferObject::create() 
	{
		if(id) throw "UniformBufferObject double create";
		glGenBuffers(1, &id);
	}
} // namespace Kirino
