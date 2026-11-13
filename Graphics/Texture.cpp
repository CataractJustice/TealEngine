#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Texture.h"
#include "libs/stb_image.h"
#include "System/Debug.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "System/Logger.h"
#include <format>

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
			this->setParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			this->setParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			Logger::logInfo(std::format("Failed to load image {}", path));
		}
		//stbi_image_free(data);
		this->pixels = data;
		this->width = width;
		this->height = height;
	}

	GLuint Texture::id() const
	{
		return this->texture;
	}


	Texture& Texture::operator=(const Texture& texture) {
		//copy fileds
		this->parameteries = texture.parameteries;
		this->dataType = texture.dataType;
		this->format = texture.format;
		this->internalformat = texture.internalformat;
		this->type = texture.type;
		this->width = texture.width;
		this->height = texture.height;

		if(texture.id() == 0) 
		{
			return *this;
		}

		// Create a buffer to hold the copied texture data
		GLubyte* data = new GLubyte[width * height * 32];
		
		glBindTexture(texture.type, texture.id()); // Bind the source texture
		
		// Get the texture data
		glGetTexImage(texture.type, 0, texture.format, texture.dataType, data);

		if (this->texture == 0)
			glGenTextures(1, &this->texture);
		glBindTexture(this->type, this->texture);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Set the texture data
		glTexImage2D(texture.type, 0, texture.internalformat, width, height, 0, texture.format, texture.dataType, data);
		
		// Clean up the buffer
		delete[] data;
		
		// Restore default texture binding
		glBindTexture(GL_TEXTURE_2D, 0);

		return *this;
	}

	Texture::Texture(const Texture& texture) {
		this->texture = 0;
		//copy fileds
		this->parameteries = texture.parameteries;
		this->dataType = texture.dataType;
		this->format = texture.format;
		this->internalformat = texture.internalformat;
		this->type = texture.type;
		this->width = texture.width;
		this->height = texture.height;

		if(texture.id() == 0) 
		{
			return;
		}
		//Create a buffer to hold the copied texture data
		GLubyte* pixels = new GLubyte[width * height * 32];
		//Bind the source texture
		glBindTexture(texture.type, texture.id()); 
		//Get the texture data
		glGetTexImage(texture.type, 0, texture.format, texture.dataType, pixels);
		this->create(width, height, pixels);
		delete[] pixels;
	}

	Texture::~Texture() 
	{
		stbi_image_free(pixels);
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
