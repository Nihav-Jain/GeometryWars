#include "pch.h"
#include "D3DShader.h"

namespace Library
{
	D3DShader::D3DShader()
	{
	}


	D3DShader::~D3DShader()
	{
	}
	void D3DShader::Init(const std::string & vPath, const std::string & fPath)
	{
		UNREFERENCED_PARAMETER(vPath);
		UNREFERENCED_PARAMETER(fPath);
	}
	void D3DShader::Use()
	{
	}
	void D3DShader::SetMatrix4(const std::string & name, const glm::mat4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}
	void D3DShader::SetVector4(const std::string & name, const glm::vec4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}
}