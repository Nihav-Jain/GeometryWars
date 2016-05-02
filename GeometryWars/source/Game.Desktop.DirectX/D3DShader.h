#pragma once
#include <Shader.h>
namespace Library
{
	class D3DShader : public Library::Shader
	{
	public:
		D3DShader();
		virtual ~D3DShader();

		virtual void Init(const std::string & vPath, const std::string & fPath) override;
		virtual void Use() override;
		virtual void SetMatrix4(const std::string & name, const glm::mat4 & value) override;
		virtual void SetVector4(const std::string & name, const glm::vec4 & value) override;
	};
}