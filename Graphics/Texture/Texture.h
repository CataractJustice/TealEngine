#pragma once
#include "libs/glm/vec4.hpp"
#include "libs/glew-2.2.0/include/GL/glew.h"
#include "libs/glfw/include/GLFW/glfw3.h"
#include <string>
#include <map>

namespace TealEngine 
{
	class Texture 
	{
	protected:
		GLuint texture;
		GLuint type, format, internalformat, dataType, width, height;
		std::map<GLenum, GLenum> parameteries;
		std::string name;
	public:
		Texture(GLuint type = GL_TEXTURE_2D, GLuint internalformat = GL_RGB, GLuint format = GL_RGB, GLuint dataType = GL_UNSIGNED_BYTE);
		~Texture();
		void generateMipmap();
		void setParameteri(GLenum pname, GLenum param);
		GLenum getParameteri(GLenum pname);
		void setBorderColor(glm::vec4 color);
		void bind(int i);
		void create(GLuint width, GLuint height, void* pixels = 0);
		void loadFromFile(std::string path);
		Texture& operator=(const Texture& t);
		GLuint id();
		inline GLuint getWidth() { return width; };
		inline GLuint getHeight() { return height; };
		inline void setName(const std::string& name) { this->name = name; };
		inline std::string getName() { return name; };
		int getInt32(int x, int y);
	};
}
