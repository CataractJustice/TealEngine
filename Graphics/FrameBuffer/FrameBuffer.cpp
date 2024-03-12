#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FrameBuffer.h"
#include "System/Debug.h"
#include "Graphics/Graphics.h"
namespace TealEngine
{
	FrameBuffer::FrameBuffer() 
	{
		FBO = 0;
	}
	void FrameBuffer::unbind()
	{
		if(bindStackSize == 0) throw;
		if(bindStackSize == 1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0); 
			glViewport(0, 0, Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			bindStackSize = 0;
			return;
		}
		bindStackSize -= 2;
		bindStack[bindStackSize]->bind();
	}
	void FrameBuffer::apply()
	{
		GLuint attachments[16];
		GLubyte p = 0;
		for (unsigned int i = 0; i < 16; i++) if (this->attachments[i])
		{
			attachments[p] = GL_COLOR_ATTACHMENT0 + i;
			p++;
		}
		bool doBind = this != currentBindedFrameBuffer();
		if(doBind) bind();
		glDrawBuffers(p, attachments);
		if(doBind) unbind();
	}

	void FrameBuffer::bind()
	{
		if(!FBO)
			glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, vw, vh);
		bindStack[bindStackSize++] = this; 
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
			bool doBind = this != currentBindedFrameBuffer();
			if(doBind) bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, texture, 0);
			if (texture != 0) 
			{
				attachments[id] = true;
				attachedTextures[id] = texture;
			}
			if(doBind) unbind();
		}
		else if(id != 255)
		{
			TE_DEBUG_ERROR("FrameBuffer::attachTexture(GLuint texture, GLubyte id) ERROR: id should be lower than 16.");
		}
	}

	void FrameBuffer::attachDepthTexture(GLuint texture) 
	{
		this->depthTexture = texture;
		bool doBind = this != currentBindedFrameBuffer();
		if(doBind) bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		if(doBind) unbind();
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

	void FrameBuffer::viewport(int w, int h) 
	{
		this->vw = w;
		this->vh = h;
		glViewport(0, 0, vw, vh);
	}

	FrameBuffer* FrameBuffer::currentBindedFrameBuffer() 
	{
		return bindStack[bindStackSize - 1];
	}

	FrameBuffer* FrameBuffer::bindStack[256] = {0};
	int FrameBuffer::bindStackSize = 0;
}