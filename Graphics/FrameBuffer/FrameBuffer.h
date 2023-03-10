#pragma once
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
		static void unbind();
		void enable(GLuint id);
		void disable(GLuint id);
		void attachTexture(GLuint texture, GLubyte id);
		void attachDepthTexture(GLuint texture);
		~FrameBuffer();
		GLuint getDepthTexture();
		GLuint getAttachedTextureId(GLuint index);
		inline GLuint getWidth() const { return width; }
		inline GLuint getHeight() const { return height; }
	};
}