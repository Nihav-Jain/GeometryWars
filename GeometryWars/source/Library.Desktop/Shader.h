#pragma once
#include "glm/glm.hpp"

namespace Library {

	class Shader
	{
	public:
		virtual ~Shader();

		virtual void Init(const std::string & vPath, const std::string & fPath, const std::string & gPath) = 0;
		virtual void Use() = 0;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) = 0;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) = 0;

	protected:
		Shader();
	};

}