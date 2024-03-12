#pragma once
namespace TealEngine 
{
	class FrameBuffer
	{
	private:
		GLuint FBO, depthTexture;
		bool attachments[16];
		GLuint attachedTextures[16];
		static FrameBuffer* bindStack[256];
		static int bindStackSize;
		int vw, vh;
	public:
		void apply();
		void bind();
		static void unbind();
		void enable(GLuint id);
		void disable(GLuint id);
		void viewport(int w, int h);
		void attachTexture(GLuint texture, GLubyte id);
		void attachDepthTexture(GLuint texture);
		static FrameBuffer* currentBindedFrameBuffer();
		FrameBuffer();
		~FrameBuffer();
		GLuint getDepthTexture();
		GLuint getAttachedTextureId(GLuint index);
	};
}