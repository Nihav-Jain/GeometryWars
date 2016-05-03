#pragma once
#include <vector>
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

		virtual void Init(std::uint32_t textureCnt, std::int32_t width, std::int32_t height) override;
		virtual void Use() override;
		virtual const std::vector<Library::Texture *> & GetFrameTexture() override;
		virtual void ClearColor(glm::vec4 color) override;

	private:
		GLuint mFBO;
		GLuint mRBO;
		std::vector<Library::Texture *> mTextures;
	};

}