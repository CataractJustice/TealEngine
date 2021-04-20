#pragma once
#include "libs/glm/vec4.hpp"
#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>
namespace TealEngine 
{
	class Texture 
	{
	private:
		GLuint texture;
		GLuint type, format, internalformat, dataType;
		std::map<GLenum, GLenum> parameteries;
	public:
		Texture(GLuint type = GL_TEXTURE_2D, GLuint internalformat = GL_RGB, GLuint format = GL_RGB, GLuint dataType = GL_UNSIGNED_BYTE);
		void generateMipmap();
		void setParameteri(GLenum pname, GLenum param);
		GLenum getParameteri(GLenum pname);
		void setBorderColor(glm::vec4 color);
		void bind(int i);
		void create(GLuint width, GLuint height, void* pixels = 0);
		void loadFromFile(std::string path);
		GLuint id();
	};
}
