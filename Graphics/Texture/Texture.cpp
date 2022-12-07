#pragma once
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Texture.h"
#include "libs/stb_image.h"

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
			
			std::cout << "Texture2D::loadFromFile(string) ERROR: failed load image \"" << path << "\".\n";
		}
		stbi_image_free(data);
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
		return *this;
	}
}
