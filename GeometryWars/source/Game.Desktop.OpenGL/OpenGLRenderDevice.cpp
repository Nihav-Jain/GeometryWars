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
		mWindow(nullptr)
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

	void OpenGLRenderDevice::InitOpenGl()
	{
		if (!glfwInit())
		{
			throw std::exception("glfwInit falied");
		}

		mWindow = glfwCreateWindow(800, 600, "OpenGL Essentials", nullptr, nullptr);
		if (mWindow == nullptr)
		{
			throw std::exception("glfwCreateWindow falied");
		}

		glfwMakeContextCurrent(mWindow);

		if (gl3wInit() != 0)
		{
			throw std::exception("gl3wInit falied");
		}

		glViewport(0, 0, 800, 600);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Viewport * OpenGLRenderDevice::CreateViewport()
	{
		return nullptr;
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
	}

	Shader * OpenGLRenderDevice::CreateShader(const std::string & vPath, const std::string & fPath)
	{
		OpenGLShader * shader = new OpenGLShader();
		shader->Init(vPath, fPath);
		mShaders.push_back(shader);
		return shader;
	}

	void OpenGLRenderDevice::Draw()
	{
		const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	RenderBuffer * OpenGLRenderDevice::CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride)
	{
		OpenGLRenderBuffer * buffer = new OpenGLRenderBuffer();
		buffer->Init(data, size, stride);
		mBuffers.push_back(buffer);
		return buffer;
	}
}
