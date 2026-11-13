#pragma once
#include <vector>
namespace Kirino 
{
	class VertexArrayObject 
	{
		public:
		struct VertexAttribPointer 
		{
			bool active;
			unsigned int buffer;
			unsigned int index;
			int size;
			unsigned int type;
			bool normalized;
			unsigned int stride;
			void* pointer;
			unsigned int divisor;
			inline VertexAttribPointer(bool active, unsigned int buffer, unsigned int index, int size, unsigned int type, bool normalized, unsigned int stride, void* pointer, unsigned int divisor) :
			active(active),
			buffer(buffer),
			index(index),
			size(size),
			type(type),
			normalized(normalized),
			stride(stride),
			pointer(pointer),
			divisor(divisor)
			{

			}
		};
		private:
		unsigned int vao;
		unsigned int ebo;

		std::vector<VertexAttribPointer> vertexAttribPointers;
		public:
		const VertexArrayObject& operator=(const VertexArrayObject&);
		inline operator unsigned int() const 
		{
			return vao;
		}
		VertexArrayObject();
		VertexArrayObject(const VertexArrayObject&);
		void vertexAttribPointer(const VertexAttribPointer&);
		void bindElementArray(unsigned int ebo);
		void updateAttribs();
		~VertexArrayObject();
		void create();
	};

}