#pragma once
#include "RenderBuffer.h"
#include "GL/gl3w.h"

namespace Library {

	class OpenGLRenderBuffer :
		public RenderBuffer
	{
	public:
		OpenGLRenderBuffer();
		~OpenGLRenderBuffer();

		virtual void Init(float * data, std::uint32_t size, std::uint32_t stride) override;
		virtual void Use() override;
	private:
		GLuint mVAO;
		GLuint mVBO;
	};

}
