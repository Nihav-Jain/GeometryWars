#include "pch.h"
#include "OpenGLBuffer.h"
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

	OpenGLRenderBuffer::OpenGLRenderBuffer() :
		mVAO(0),
		mVBO(0),
		mEBO(0)
	{
	}


	OpenGLRenderBuffer::~OpenGLRenderBuffer()
	{
		if (mVAO != 0) {
			glDeleteVertexArrays(1, &mVAO);
			mVAO = 0;
		}

		if (mVBO != 0) {
			glDeleteBuffers(1,&mVBO);
			mVBO = 0;
		}

		if (mEBO != 0) {
			glDeleteBuffers(1, &mEBO);
			mEBO = 0;
		}
	}

	void OpenGLRenderBuffer::SetData(float * data, std::uint32_t size, std::uint32_t stride,
		std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt)
	{
		GLfloat * vertices = data;
		GLuint strideSize = stride;

		glBindVertexArray(mVAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		if (mEBO != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, elementCnt, GL_FLOAT, GL_FALSE, strideSize, (GLvoid*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLRenderBuffer::Init(bool createIndicesBuffer)
	{
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);

		if (createIndicesBuffer) {
			glGenBuffers(1, &mEBO);
		}

	}

	void OpenGLRenderBuffer::Use()
	{
		glBindVertexArray(mVAO);
	}

}
