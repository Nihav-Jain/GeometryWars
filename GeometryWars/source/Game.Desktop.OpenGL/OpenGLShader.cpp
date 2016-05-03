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

namespace OpenGLImplmentation {

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

	void OpenGLShader::Init(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		GLint success = 0;
		GLuint fragmentShader = createOpenGLShaderObj(GL_FRAGMENT_SHADER, fPath);
		GLuint vertexShader = createOpenGLShaderObj(GL_VERTEX_SHADER, vPath);
		GLuint geomatryShader = createOpenGLShaderObj(GL_GEOMETRY_SHADER, gPath);

		mShaderId = glCreateProgram();

		if (vertexShader != 0) glAttachShader(mShaderId, vertexShader);
		if (fragmentShader != 0) glAttachShader(mShaderId, fragmentShader);
		if (geomatryShader != 0) glAttachShader(mShaderId, geomatryShader);
		glLinkProgram(mShaderId);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glGetProgramiv(mShaderId, GL_LINK_STATUS, &success);
		if (!success) {
			char buf[256] = { 0 };
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
		glUniform4f(glGetUniformLocation(mShaderId, name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetFloat(const std::string & name, float value)
	{
		glUniform1f(glGetUniformLocation(mShaderId, name.c_str()), value);
	}

	void OpenGLShader::SetBool(const std::string & name, bool value)
	{
		glUniform1i(glGetUniformLocation(mShaderId, name.c_str()), value);
	}

	void OpenGLShader::SetInt(const std::string & name, std::int32_t value)
	{
		glUniform1i(glGetUniformLocation(mShaderId, name.c_str()), value);
	}

	GLuint OpenGLShader::createOpenGLShaderObj(GLuint type, const std::string & path)
	{
		GLuint shadeId = 0;

		if (path != "") {
			std::ifstream t;
			t.open(path);
			std::string shader((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			t.close();

			GLint success = 0;
			char buf[256] = { 0 };

			const char *f_str = shader.c_str();
			shadeId = glCreateShader(type);
			glShaderSource(shadeId, 1, &f_str, NULL);
			glCompileShader(shadeId);

			glGetShaderiv(shadeId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shadeId, 1024, NULL, buf);
				printf("%s", buf);
				return 0;
			}
		}

		return shadeId;
	}

}