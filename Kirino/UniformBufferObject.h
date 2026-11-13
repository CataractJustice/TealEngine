#pragma once
#include <GL/glew.h>
#include <memory.h>
namespace Kirino 
{
	class UniformBufferObject 
	{
		private:
		unsigned int id;
		public:
		template<class T>
		void upload(T data) 
		{
			glBindBuffer(GL_UNIFORM_BUFFER, id);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
		}
		UniformBufferObject();
		~UniformBufferObject();
		void create();
		inline unsigned int getId() { return id; }
	};
}