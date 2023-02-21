#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Texture.h"
#include "libs/stb_image.h"
#include "System/Debug.h"

namespace TealEngine
{

	Texture::Texture(GLuint type, GLuint internalformat, GLuint format, GLuint dataType)
	{
		this->texture = 0;
		this->type = type;
		this->internalformat = internalformat;
		this->format = format;
		this->dataType = dataType;
	}

	void Texture::setParameteri(GLenum pname, GLenum param)
	{
		this->parameteries[pname] = param;
		glBindTexture(this->type, texture);
		glTexParameteri(this->type, pname, param);
		glBindTexture(this->type, 0);
	}

	GLenum Texture::getParameteri(GLenum pname)
	{
		return parameteries[pname];
	}

	void Texture::setBorderColor(glm::vec4 color) 
	{
		glBindTexture(this->type, texture);
		GLfloat c[4] = {color.r, color.g, color.b, color.r};
		glTexParameterfv(this->type, GL_TEXTURE_BORDER_COLOR, c);
	}

	void Texture::generateMipmap() 
	{
		glBindTexture(this->type, texture);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(this->type);
	}

	void Texture::bind(int i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(this->type, texture);
	}

	void Texture::create(GLuint width, GLuint height, void* pixels)
	{
		if (texture == 0)
			glGenTextures(1, &texture);
		glBindTexture(this->type, texture);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(this->type, 0, this->internalformat, width, height, 0, this->format, dataType, pixels);
		glBindTexture(this->type, 0);
		this->width = width;
		this->height = height;
	}

	void Texture::loadFromFile(std::string path)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (nrChannels == 4) 
		{
			this->format = GL_RGBA;
			this->internalformat = GL_RGBA;
		}
		else if (nrChannels == 3) {
			this->format = GL_RGB;
			this->internalformat = GL_RGB;
		}
		if (data)
		{
			this->create(width, height, data);
		}
		else
		{
			TE_DEBUG_ERROR("Failed to load image");
		}
		stbi_image_free(data);
		this->width = width;
		this->height = height;
	}

	GLuint Texture::id()
	{
		return this->texture;
	}

	Texture& Texture::operator=(const Texture& texture) {
		glDeleteTextures(1, &this->texture);
		this->parameteries = texture.parameteries;
		this->dataType = texture.dataType;
		this->format = texture.format;
		this->internalformat = texture.internalformat;
		this->texture = texture.texture;
		this->type = texture.type;
		this->width = texture.width;
		this->height = texture.height;
		return *this;
	}

	Texture::~Texture() 
	{
		glDeleteTextures(1, &this->texture);
	}
	
	int Texture::getInt32(int x, int y) 
	{
		int* pixels = new int[width * height];
		glBindTexture(this->type, this->texture);
		glGetTexImage(this->type, 0, GL_RED_INTEGER, GL_INT, pixels);
		int p = ((int*)pixels)[x + (height - y - 1) * width];
		delete pixels;
		return p;
	}
}
