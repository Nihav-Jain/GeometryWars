#include "pch.h"
#include "OpenGLShader.h"
#include "OpenGLRenderDevice.h"
#include "GL/gl3w.h"
#include "SOIL/SOIL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Library {

	OpenGLShader::OpenGLShader() :
		mShaderId(0)
	{
	}


	OpenGLShader::~OpenGLShader()
	{
		if (mShaderId != 0) {
			glDeleteProgram(mShaderId);
			mShaderId = 0;
		}
	}

	void OpenGLShader::Init(const std::string & vPath, const std::string & fPath)
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

		mShaderId = glCreateProgram();

		glAttachShader(mShaderId, vertexShader);
		glAttachShader(mShaderId, fragmentShader);
		glLinkProgram(mShaderId);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(mShaderId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(mShaderId, 512, NULL, buf);
			printf("%s", buf);
		}
	}

	void OpenGLShader::Use()
	{
		glUseProgram(mShaderId);
	}

	void OpenGLShader::SetMatrix4(const std::string & name, const glm::mat4 & value)
	{
		glUniformMatrix4fv(glGetUniformLocation(mShaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetVector4(const std::string & name, const glm::vec4 & value)
	{
		glUniform3f(glGetUniformLocation(mShaderId, name.c_str()), value.x, value.y, value.z);
	}

}