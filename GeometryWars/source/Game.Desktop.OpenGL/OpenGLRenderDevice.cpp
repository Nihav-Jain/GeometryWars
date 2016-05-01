#include "pch.h"
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

	OpenGLRenderDevice::OpenGLRenderDevice() :
		mWindow(nullptr),
		mWidth(800),
		mHeight(600)
	{
	}


	OpenGLRenderDevice::~OpenGLRenderDevice()
	{
		for (auto ptr : mShaders) {
			delete ptr;
		}

		for (auto ptr : mBuffers) {
			delete ptr;
		}

		for (auto ptr : mTextures) {
			delete ptr;
		}

		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void OpenGLRenderDevice::InitOpenGl(std::int32_t width, std::int32_t height)
	{
		mWidth = width;
		mHeight = height;

		if (mWidth < 400)
			mWidth = 400;
		if (mHeight < 300)
			mHeight = 300;

		if (!glfwInit())
		{
			throw std::exception("glfwInit falied");
		}

		mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL Essentials", nullptr, nullptr);
		if (mWindow == nullptr)
		{
			throw std::exception("glfwCreateWindow falied");
		}

		glfwMakeContextCurrent(mWindow);

		if (gl3wInit() != 0)
		{
			throw std::exception("gl3wInit falied");
		}

		glViewport(0, 0, mWidth, mHeight);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	std::int32_t OpenGLRenderDevice::GetViewportWidth()
	{
		return mWidth;
	}

	std::int32_t OpenGLRenderDevice::GetViewportHeight()
	{
		return mHeight;
	}

	Texture * OpenGLRenderDevice::CreateTexture(const std::string & imagePath)
	{
		OpenGLTexture * texture = new OpenGLTexture();
		texture->Init(imagePath);
		mTextures.push_back(texture);
		return texture;
	}

	void OpenGLRenderDevice::Invalid()
	{
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
		const glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glClearBufferfv(GL_COLOR, 0, &Black[0]);
	}

	Shader * OpenGLRenderDevice::CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		OpenGLShader * shader = new OpenGLShader();
		shader->Init(vPath, fPath, gPath);
		mShaders.push_back(shader);
		return shader;
	}

	void OpenGLRenderDevice::Draw(DrawMode mode, std::uint32_t counts)
	{
		switch (mode) {
		case DrawMode::TRIANGLES:
			glDrawArrays(GL_TRIANGLES, 0, counts);
			break;
		case DrawMode::LINES:
			glDrawElements(GL_LINES, counts, GL_UNSIGNED_INT, 0);
			break;
		case DrawMode::POINTS:
			glDrawArrays(GL_POINTS, 0, counts);
			break;
		default:
			break;
		}
		
		glBindVertexArray(0);
	}

	RenderBuffer * OpenGLRenderDevice::CreateBuffer(bool createIndicesBuffer)
	{
		OpenGLRenderBuffer * buffer = new OpenGLRenderBuffer();
		buffer->Init(createIndicesBuffer);
		mBuffers.push_back(buffer);
		return buffer;
	}
}
