#pragma once
#include "Buffer.h"
#include "GL/gl3w.h"

namespace OpenGLImplmentation {

	class OpenGLRenderBuffer :
		public Library::Buffer
	{
	public:
		OpenGLRenderBuffer();
		~OpenGLRenderBuffer();

		virtual void Init(bool createIndicesBuffer) override;
		virtual void SetData(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt) override;
		virtual void Use() override;
	private:
		GLuint mVAO;
		GLuint mVBO;
		GLuint mEBO;
	};

}
