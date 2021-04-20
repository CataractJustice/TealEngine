#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
namespace TealEngine 
{
	class FrameBuffer
	{
	private:
		GLuint FBO, width, height, depthTexture;
		bool attachments[16];
		GLuint attachedTextures[16];
	public:
		void viewport(GLuint w, GLuint h);
		void resize(GLuint width, GLuint height);
		void apply();
		void bind();
		static void unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void enable(GLuint id);
		void disable(GLuint id);
		void attachTexture(GLuint texture, GLubyte id);
		void attachDepthTexture(GLuint texture);
		~FrameBuffer() 
		{
			glDeleteFramebuffers(1, &this->FBO);
		}
		GLuint getDepthTexture();
		GLuint getAttachedTextureId(GLuint index);
	};
}