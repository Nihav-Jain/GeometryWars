#include "pch.h"
#include "OpenGLRenderDevice.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Library {

	OpenGLRenderDevice::OpenGLRenderDevice() :
		mWindow(nullptr)
	{
	}


	OpenGLRenderDevice::~OpenGLRenderDevice()
	{
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
	}

	Viewport * OpenGLRenderDevice::CreateViewport()
	{
		return nullptr;
	}

	void OpenGLRenderDevice::Draw()
	{
		const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	bool OpenGLRenderDevice::IsValid()
	{
		return !glfwWindowShouldClose(mWindow);
	}
}
