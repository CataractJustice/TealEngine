#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace TealEngine 
{
	struct VertexAttribute 
	{
		GLuint index;
		GLuint size;
		GLenum type;
		bool enabled;
		unsigned int bytesize;
		unsigned int id;
		void* data;
		unsigned int vertices;
	};

	class VertexBuffer 
	{
		private:
		GLuint VAO,VBO;
		GLenum bufferUsage;
		std::vector<VertexAttribute> vertexAttributes;
		char* buffer;
		unsigned int bufferSize;
		public:
		inline GLuint getVAO() const {return VAO;};
		inline GLuint getVBO() const {return VBO;};
		inline GLenum getBufferUsage() const {return bufferUsage;};
		inline void setBufferUsage(GLenum usage) {this->bufferUsage = usage;};
		bool isVertexAttributeEnabled(GLuint layoutIndex) const;

		VertexAttribute& addVertexAttribute(GLuint layoutIndex, GLuint size, GLenum type = GL_FLOAT);
		void removeVertexAttribute(const VertexAttribute& vertexAttribute);
		inline void clearVertexAttributes() {this->vertexAttributes.clear();}
		void enableVertexAttribute(const VertexAttribute& vertexAttribute);
		void disableVertexAttribute(const VertexAttribute& vertexAttribute);
		void build();

		VertexBuffer();
		~VertexBuffer();
	};
}