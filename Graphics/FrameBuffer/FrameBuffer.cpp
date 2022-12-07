#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FrameBuffer.h"
#include "System/Debug.h"
namespace TealEngine
{
	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::resize(GLuint width, GLuint height) 
	{
		glDeleteFramebuffers(1, &FBO);
		viewport(width, height);
		glGenFramebuffers(1, &FBO);
	}

	void FrameBuffer::viewport(GLuint w, GLuint h)
	{
		this->width = w;
		this->height = h;
	}

	void FrameBuffer::apply()
	{
		GLuint* attachments = new GLuint[16];
		GLubyte p = 0;
		for (unsigned int i = 0; i < 16; i++) if (this->attachments[i])
		{
			attachments[p] = GL_COLOR_ATTACHMENT0 + i;
			p++;
		}
		bind();
		glDrawBuffers(p, attachments);
		delete[] attachments;
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, width, height);
	}

	void FrameBuffer::enable(GLuint id) 
	{
		attachTexture(attachedTextures[id], id);
	}

	void FrameBuffer::disable(GLuint id)
	{
		attachTexture(0, id);
	}

	void FrameBuffer::attachTexture(GLuint texture, GLubyte id)
	{
		if (id < 16)
		{
			bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, texture, 0);
			if (texture != 0) 
			{
				attachments[id] = true;
				attachedTextures[id] = texture;
			}
		}
		else if(id != 255)
		{
			TE_DEBUG_ERROR("FrameBuffer::attachTexture(GLuint texture, GLubyte id) ERROR: id should be lower than 16.");
		}
	}

	void FrameBuffer::attachDepthTexture(GLuint texture) 
	{
		this->depthTexture = texture;
		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	}

	GLuint FrameBuffer::getDepthTexture() 
	{
		return this->depthTexture;
	}

	GLuint FrameBuffer::getAttachedTextureId(GLuint index) 
	{
		return this->attachments[index];
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &this->FBO);
	}
}