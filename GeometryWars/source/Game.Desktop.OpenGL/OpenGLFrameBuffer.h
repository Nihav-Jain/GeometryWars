#pragma once
#include "FrameBuffer.h"
#include "GL/gl3w.h"


namespace OpenGLImplmentation {

	class OpenGLTexture;

	class OpenGLFrameBuffer :
		public Library::FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		OpenGLFrameBuffer(GLuint id);
		~OpenGLFrameBuffer();

		virtual void Init(std::int32_t width, std::int32_t height) override;
		virtual void Use() override;
		virtual Library::Texture * GetFrameTexture() override;

	private:
		GLuint mFBO;
		OpenGLTexture * mTexture;
	};

}