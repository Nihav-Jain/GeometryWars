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
		mRBO(0)
	{
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(GLuint id) :
		mFBO(id),
		mRBO(0)
	{
	}


	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		if (mFBO != 0)
			glDeleteFramebuffers(1, &mFBO);
	
		for (auto & t : mTextures) {
			delete t;
		}

		if (mRBO != 0)
			glDeleteRenderbuffers(1, &mRBO);
	}

	void OpenGLFrameBuffer::Init(std::uint32_t textureCnt, std::int32_t width, std::int32_t height)
	{
		glGenFramebuffers(1, &mFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		for (std::uint32_t i = 0; i < textureCnt; i++) {
			GLuint textureId = 0;
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureId, 0);

			mTextures.push_back(new OpenGLTexture(textureId));
		}

		glGenRenderbuffers(1, &mRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::exception("Error");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Use()
	{
		if (mFBO != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

			auto rt = glGetError();
			if (rt != GL_NO_ERROR) {
				throw std::exception("Error");
			}
		
			GLenum buff[16] = { 0 };
			for (std::uint32_t i = 0; i < mTextures.size(); i++) {
				buff[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			glDrawBuffers((GLsizei)mTextures.size(), buff);
		}
		else {

			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

			auto rt = glGetError();
			if (rt != GL_NO_ERROR) {
				throw std::exception("Error");
			}

			GLenum buff[] = { GL_BACK_LEFT };
			glDrawBuffers(1, buff);
		}
	}

	const std::vector<Library::Texture *> & OpenGLFrameBuffer::GetFrameTexture()
	{
		return mTextures;
	}
	void OpenGLFrameBuffer::ClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}