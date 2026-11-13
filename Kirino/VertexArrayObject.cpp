#include "VertexArrayObject.h"
#include <GL/glew.h>
#include <stdexcept>
namespace Kirino
{
	VertexArrayObject::VertexArrayObject() 
	{
		vao = 0;
		ebo = 0;
	}

	void VertexArrayObject::create() 
	{
		if(vao) throw std::runtime_error("VAO already created");
		glGenVertexArrays(1, &vao);
		int maxAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
		VertexAttribPointer vap(false, 0, 0, 0, 0, false, 0, nullptr, 0);
		vertexAttribPointers.reserve(maxAttribs);
		for(int i = 0; i < maxAttribs; i++) vertexAttribPointers.push_back(vap);
	}

	const VertexArrayObject& VertexArrayObject::operator=(const VertexArrayObject& val) 
	{
		if(vao == 0) 
		{
			glGenVertexArrays(1, &vao);
		}
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, val.ebo);
		ebo = val.ebo;
		vertexAttribPointers = val.vertexAttribPointers;
		updateAttribs();
		return *this;
	}

	VertexArrayObject::VertexArrayObject(const VertexArrayObject& val) 
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, val.ebo);
		ebo = val.ebo;
		vertexAttribPointers = val.vertexAttribPointers;
		updateAttribs();
	}

	void VertexArrayObject::vertexAttribPointer(const VertexArrayObject::VertexAttribPointer& vap) 
	{
		vertexAttribPointers[vap.index] = vap;
	}

	void VertexArrayObject::updateAttribs() 
	{
		glBindVertexArray(vao);
		int lastBind = -1;
		for(const auto& vap : vertexAttribPointers) 
		{
			if(!vap.active) continue;
			if(lastBind != vap.buffer) 
			{
				glBindBuffer(GL_ARRAY_BUFFER, vap.buffer);
				lastBind = vap.buffer;
			}
			if(vap.type == GL_UNSIGNED_INT || vap.type == GL_UNSIGNED_SHORT) 
			{
				glVertexAttribIPointer(vap.index, vap.size, vap.type, vap.stride, vap.pointer);
			} else 
			{
				glVertexAttribPointer(vap.index, vap.size, vap.type, vap.normalized, vap.stride, vap.pointer);
			}
			glEnableVertexAttribArray(vap.index);
			glVertexAttribDivisor(vap.index, vap.divisor);
		}
	}

	void VertexArrayObject::bindElementArray(unsigned int ebo) 
	{
		this->ebo = ebo;
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	VertexArrayObject::~VertexArrayObject() 
	{
		if(vao) glDeleteVertexArrays(1, &vao);
	}
} // namespace Kirino
