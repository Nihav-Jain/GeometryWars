#pragma once
#include "Shader.h"
#include "GL/gl3w.h"
#include <iostream>
#include <fstream>
#include <streambuf>

namespace Library {

	class OpenGLShader :
		public Shader
	{

	public:
		OpenGLShader();
		~OpenGLShader();

		void Init(const std::string & vPath, const std::string & fPath);

		virtual void Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;

	private:
		GLuint mShaderId;
	};

}
