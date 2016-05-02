#pragma once
#include "Texture.h"
#include "GL/gl3w.h"

namespace OpenGLImplmentation {

	class OpenGLTexture :
		public Library::Texture
	{

	public:
		OpenGLTexture();
		~OpenGLTexture();

		virtual void Init(const std::string & imagePath) override;
		virtual void Use() override;
	private:
		GLuint mTextureId;
	};

}
