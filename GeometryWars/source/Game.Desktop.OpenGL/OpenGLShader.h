#pragma once
#include "Shader.h"
#include "GL/gl3w.h"
#include <iostream>
#include <fstream>
#include <streambuf>

namespace OpenGLImplmentation {

	class OpenGLShader :
		public Library::Shader
	{

	public:
		OpenGLShader();
		~OpenGLShader();

		virtual void Init(const std::string & vPath, const std::string & fPath, const std::string & gPath) override;
		virtual void Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;
		virtual void SetFloat(const std::string & name, float value) override;
		virtual void SetBool(const std::string & name, bool value) override;
		virtual void SetInt(const std::string & name, std::int32_t value) override;
	private:
		GLuint mShaderId;

		GLuint createOpenGLShaderObj(GLuint type, const std::string & path);
	};

}
