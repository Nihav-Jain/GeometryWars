#include "pch.h"
#include "OpenGLRenderBuffer.h"
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

namespace Library {

	OpenGLRenderBuffer::OpenGLRenderBuffer() :
		mVAO(0)
	{
	}


	OpenGLRenderBuffer::~OpenGLRenderBuffer()
	{
	}

	void OpenGLRenderBuffer::Init(float * data, std::uint32_t size, std::uint32_t stride)
	{
		GLuint VBO;
		GLfloat * vertices = data;
		GLuint strideSize = stride;

		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		glBindVertexArray(mVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strideSize, (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void OpenGLRenderBuffer::Use()
	{
		glBindVertexArray(mVAO);
	}

}
