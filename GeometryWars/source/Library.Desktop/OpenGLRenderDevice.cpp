#include "pch.h"
#include "OpenGLRenderDevice.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Library {

	OpenGLRenderDevice::OpenGLRenderDevice()
	{
	}


	OpenGLRenderDevice::~OpenGLRenderDevice()
	{
		/*
			while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
		*/
	}

	void OpenGLRenderDevice::InitOpenGl()
	{
		const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

		if (!glfwInit())
		{
			throw std::exception("glfwInit falied");
		}

		GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Essentials", nullptr, nullptr);
		if (window == nullptr)
		{
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (gl3wInit() != 0)
		{
			return -1;
		}

		glViewport(0, 0, 800, 600);
	}

	Viewport * OpenGLRenderDevice::CreateViewport()
	{
		return nullptr;
	}

}
