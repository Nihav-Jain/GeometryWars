#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"
#include "GL/gl3w.h"
#include "SOIL/SOIL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGLImplmentation {
	OpenGLFrameBuffer::OpenGLFrameBuffer() :
		mFBO(0),
		mTexture(nullptr)
	{
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(GLuint id) :
		mFBO(id),
		mTexture(nullptr)
	{
	}


	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		if (mFBO != 0)
			glDeleteFramebuffers(1, &mFBO);
		if (mTexture != 0)
			delete mTexture;
	}

	void OpenGLFrameBuffer::Init(std::int32_t width, std::int32_t height)
	{
		GLuint textureId = 0;
		glGenBuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::exception("Error");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		mTexture = new OpenGLTexture(textureId);
	}
	void OpenGLFrameBuffer::Use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	}

	Library::Texture * OpenGLFrameBuffer::GetFrameTexture()
	{
		return mTexture;
	}
}