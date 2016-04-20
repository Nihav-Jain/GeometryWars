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

	void OpenGLRenderDevice::Invalid()
	{
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	bool OpenGLRenderDevice::IsValid()
	{
		return !glfwWindowShouldClose(mWindow);
	}

	std::uint32_t OpenGLRenderDevice::LoadTexture(const std::string & imagePath)
	{
		GLuint textureID = SOIL_load_OGL_texture
			(
				imagePath.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y
				);

		std::string result(SOIL_last_result());
		printf("%s", result.c_str());

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		// TODO: make sure the cast is safe
		return textureID;
	}

	std::uint32_t OpenGLRenderDevice::LoadShader(const std::string & vPath, const std::string & fPath)
	{
		std::ifstream t;
		t.open(fPath);
		std::string fShader((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		t.close();

		t.open(vPath);
		std::string vShader((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		t.close();

		GLint success = 0;
		char buf[256] = { 0 };

		GLuint fragmentShader = 0;
		
		const char *f_str = fShader.c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &f_str, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 1024, NULL, buf);
			printf("%s", buf);
		}

		GLuint vertexShader = 0;
		const char *v_str = vShader.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &v_str, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 1024, NULL, buf);
			printf("%s", buf);
		}

		GLuint shaderProgram = 0;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, buf);
			printf("%s", buf);
		}

		return shaderProgram;
	}

	void OpenGLRenderDevice::UseTexture(std::uint32_t texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void OpenGLRenderDevice::UseShader(std::uint32_t shaderProgram)
	{
		glUseProgram(shaderProgram);
	}

	void OpenGLRenderDevice::Draw()
	{
		const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	std::uint32_t OpenGLRenderDevice::CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride)
	{
		GLuint VAO;
		GLuint VBO;
		GLfloat * vertices = data;
		GLuint strideSize = stride;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strideSize, (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return VAO;
	}

	void OpenGLRenderDevice::UseBuffer(std::uint32_t buffer)
	{
		glBindVertexArray(buffer);
	}

	void OpenGLRenderDevice::SetShaderMatrix4(std::uint32_t id, const std::string & name, const glm::mat4 & value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLRenderDevice::SetShaderVector4(std::uint32_t id, const std::string & name, const glm::vec4 & value)
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
	}
}
