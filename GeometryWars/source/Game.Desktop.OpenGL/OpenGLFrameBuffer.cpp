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
		mRBO(0),
		mTexture(nullptr)
	{
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(GLuint id) :
		mFBO(id),
		mRBO(0),
		mTexture(nullptr)
	{
	}


	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		if (mFBO != 0)
			glDeleteFramebuffers(1, &mFBO);
		if (mTexture != 0)
			delete mTexture;
		if (mRBO != 0)
			glDeleteRenderbuffers(1, &mRBO);
	}

	void OpenGLFrameBuffer::Init(std::int32_t width, std::int32_t height)
	{
		GLuint textureId = 0;
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &mRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::exception("Error");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		mTexture = new OpenGLTexture(textureId);
	}
	void OpenGLFrameBuffer::Use()
	{
		if (mFBO != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

			auto rt = glGetError();
			if (rt != GL_NO_ERROR) {
				throw std::exception("Error");
			}
		
			GLenum buff[] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, buff);

			glEnable(GL_DEPTH_TEST);
		}
		else {

			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

			glDisable(GL_DEPTH_TEST);
			GLenum buff[] = { GL_BACK_LEFT };
			glDrawBuffers(1, buff);
		}
	}

	Library::Texture * OpenGLFrameBuffer::GetFrameTexture()
	{
		return mTexture;
	}
	void OpenGLFrameBuffer::ClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}