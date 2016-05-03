#include "pch.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderDevice.h"
#include "GL/gl3w.h"
#include "SOIL/SOIL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGLImplmentation {

	OpenGLTexture::OpenGLTexture() :
		mTextureId(0)
	{
	}

	OpenGLTexture::OpenGLTexture(GLuint textureId)
	{
		mTextureId = textureId;
	}


	OpenGLTexture::~OpenGLTexture()
	{
		if (mTextureId != 0) {
			glDeleteTextures(1, &mTextureId);
			mTextureId = 0;
		}
	}

	void OpenGLTexture::Init(const std::string & imagePath)
	{
		mTextureId = SOIL_load_OGL_texture
			(
				imagePath.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y
				);

		std::string result(SOIL_last_result());
		printf("%s", result.c_str());

		glBindTexture(GL_TEXTURE_2D, mTextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void OpenGLTexture::Use(std::uint32_t useAsTextureIndex)
	{
		glActiveTexture(GL_TEXTURE0 + useAsTextureIndex);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
	}

}